#include <iostream>
#include <string>
#include <cstdint>
#include <cstddef>
#include <memory>
#include <vector>
#include <utility>
#include <ostream>
#include <fstream>
#include <filesystem>
#include <optional>
#include <cstring>
#include <exception>

void to_lower(std::string& word) {
    for (char& c : word) {
        if ((c >= 'A') && (c <= 'Z')) {
            c = static_cast<char>(c - 'A' + 'a');
        }
    }
}

class Entry final {
private:
    std::string word_;
    std::uint64_t value_;
public:
    Entry(std::string word, std::uint64_t value)
        : word_(std::move(word)), value_(value) {
        to_lower(word_);
    }
    explicit Entry(std::string word) 
        : Entry(std::move(word), 0) {}
    const std::string& word() const {
        return word_;
    }
    std::uint64_t value() const {
        return value_;
    }
    bool operator<(const Entry& other) const {
        return word_ < other.word_;
    }
    bool operator>(const Entry& other) const {
        return other < *this;
    }
    bool operator==(const Entry& other) const {
        return word_ == other.word_;
    }
    friend std::ostream& operator<<(std::ostream& out, const Entry& entry) {
        const std::uint16_t length = static_cast<std::uint16_t>(entry.word_.size());
        out.write(reinterpret_cast<const char*>(&length), sizeof(length));
        out.write(entry.word_.data(), length);
        out.write(reinterpret_cast<const char*>(&entry.value_), sizeof(entry.value_));
        return out;
    }
    friend std::istream& operator>>(std::istream& in, Entry& entry) {
        std::uint16_t length = 0;
        in.read(reinterpret_cast<char*>(&length), sizeof(length));
        if (!in) {
            return in;
        }
        if ((length == 0) || (length > 256)) {
            in.setstate(std::ios::failbit);
            return in;
        }
        std::string word(length, '\0');
        std::uint64_t value = 0;
        in.read(word.data(), length);
        in.read(reinterpret_cast<char*>(&value), sizeof(value));
        if (!in) {
            return in;
        }
        for (char c : word) {
            if ((c < 'a') || (c > 'z')) {
                in.setstate(std::ios::failbit);
                return in;
            }
        }
        entry = Entry(std::move(word), value);
        return in;
    }
};

class BTree final {
private:

    static constexpr std::size_t t = 16;
    static constexpr std::size_t min_keys = t - 1;
    static constexpr std::size_t max_keys = 2 * t - 1;

    struct Node {
        std::vector<Entry> keys;
        std::vector<std::unique_ptr<Node>> children;
    };

    static std::size_t key_index(const Node& node, const Entry& target) {
        std::size_t i = 0;
        while ((i < node.keys.size()) && (node.keys[i] < target)) {
            ++i;
        }
        return i;
    }

    static void split_child(Node& parent, std::size_t i) {
        Node& child = *parent.children[i];
        Node new_node;
        for (std::size_t j = t; j < max_keys; ++j) {
            new_node.keys.push_back(std::move(child.keys[j]));
        }
        Entry median = std::move(child.keys[t - 1]);
        child.keys.erase(child.keys.begin() + (t - 1), child.keys.end());
        if (!child.children.empty()) {
            for (std::size_t j = t; j < max_keys + 1; ++j) {
                new_node.children.push_back(std::move(child.children[j]));
            }
            child.children.erase(child.children.begin() + t, child.children.end());
        }
        parent.keys.insert(parent.keys.begin() + i, std::move(median));
        parent.children.insert(parent.children.begin() + i + 1, std::make_unique<Node>(std::move(new_node)));
    }

    static void insert_non_full(Node& node, Entry entry) {
        std::size_t i = key_index(node, entry);
        if (node.children.empty()) {
            node.keys.insert(node.keys.begin() + i, std::move(entry));
            return;
        }
        if (node.children[i]->keys.size() == max_keys) {
            split_child(node, i);
            if (entry > node.keys[i]) {
                ++i;
            }
        }
        insert_non_full(*node.children[i], std::move(entry));
    }

    static void borrow_from_left(Node& parent, std::size_t i) {
        Node& child = *parent.children[i];
        Node& left = *parent.children[i - 1];
        child.keys.insert(child.keys.begin(), std::move(parent.keys[i - 1]));
        parent.keys[i - 1] = std::move(left.keys.back());
        left.keys.pop_back();
        if (!left.children.empty()) {
            child.children.insert(child.children.begin(), std::move(left.children.back()));
            left.children.pop_back();
        }
    }

    static void borrow_from_right(Node& parent, std::size_t i) {
        Node& child = *parent.children[i];
        Node& right = *parent.children[i + 1];
        child.keys.push_back(std::move(parent.keys[i]));
        parent.keys[i] = std::move(right.keys.front());
        right.keys.erase(right.keys.begin());
        if (!right.children.empty()) {
            child.children.push_back(std::move(right.children.front()));
            right.children.erase(right.children.begin());
        }
    }

    static void merge_children(Node& parent, std::size_t i) {
        Node& left = *parent.children[i];
        Node& right = *parent.children[i + 1];
        left.keys.push_back(std::move(parent.keys[i]));
        for (Entry& key : right.keys) {
            left.keys.push_back(std::move(key));
        }
        for (auto& child : right.children) {
            left.children.push_back(std::move(child));
        }
        parent.keys.erase(parent.keys.begin() + i);
        parent.children.erase(parent.children.begin() + i + 1);
    }

    static Entry maximum(const Node& node) {
        const Node* current = &node;
        while (!current->children.empty()) {
            current = current->children.back().get();
        }
        return current->keys.back();
    }

    static Entry minimum(const Node& node) {
        const Node* current = &node;
        while (!current->children.empty()) {
            current = current->children.front().get();
        }
        return current->keys.front();
    }

    static void erase_from(Node& node, const Entry& target) {
        std::size_t i = key_index(node, target);
        if ((i < node.keys.size()) && (node.keys[i] == target)) {
            if (node.children.empty()) {
                node.keys.erase(node.keys.begin() + i);
                return;
            }
            if (node.children[i]->keys.size() >= t) {
                Entry pred = maximum(*node.children[i]);
                erase_from(*node.children[i], pred);
                node.keys[i] = std::move(pred);
            } else if (node.children[i + 1]->keys.size() >= t) {
                Entry succ = minimum(*node.children[i + 1]);
                erase_from(*node.children[i + 1], succ);
                node.keys[i] = std::move(succ);
            } else {
                merge_children(node, i);
                erase_from(*node.children[i], target);
            }
            return;
        }
        if (node.children.empty()) {
            return;
        }
        if (node.children[i]->keys.size() == min_keys) {
            if ((i > 0) && (node.children[i - 1]->keys.size() >= t)) {
                borrow_from_left(node, i);
            } else if ((i < node.keys.size()) && (node.children[i + 1]->keys.size() >= t)) {
                borrow_from_right(node, i);
            } else if (i < node.keys.size()) {
                merge_children(node, i);
            } else {
                merge_children(node, i - 1);
                --i;
            }
        }
        erase_from(*node.children[i], target);
    }

    static void write_node(std::ostream& out, const Node& node) {
        const std::uint8_t count = static_cast<std::uint8_t>(node.keys.size());
        const std::uint8_t leaf = static_cast<std::uint8_t>(node.children.empty());
        out.write(reinterpret_cast<const char*>(&count), sizeof(count));
        out.write(reinterpret_cast<const char*>(&leaf), sizeof(leaf));
        for (const Entry& entry : node.keys) {
            out << entry;
        }
        for (const auto& child : node.children) {
            write_node(out, *child);
        }
    }

    static std::unique_ptr<Node> read_node(std::istream& in, bool is_root, const Entry* lower, const Entry* upper, std::size_t depth, std::optional<std::size_t>& leaf_depth) {
        std::uint8_t count = 0;
        std::uint8_t leaf = 0;
        in.read(reinterpret_cast<char*>(&count), sizeof(count));
        in.read(reinterpret_cast<char*>(&leaf), sizeof(leaf));
        if (!in) return nullptr;
        if ((count < (is_root ? 1 : min_keys)) || (count > max_keys) || (leaf > 1) || (depth > 64)) {
            in.setstate(std::ios::failbit);
            return nullptr;
        }
        auto node = std::make_unique<Node>();
        node->keys.reserve(count);
        if (leaf == 0) {
            node->children.reserve(static_cast<std::size_t>(count) + 1);
        }
        for (std::size_t i = 0; i < count; ++i) {
            Entry entry("");
            if (!(in >> entry)) {
                return nullptr;
            }
            if ((lower && (!(*lower < entry))) || (upper && (!(*upper > entry))) || ((!node->keys.empty()) && (!(node->keys.back() < entry)))) {
                in.setstate(std::ios::failbit);
                return nullptr;
            }
            node->keys.push_back(std::move(entry));
        }
        if (leaf == 1) {
            if (!leaf_depth.has_value()) {
                leaf_depth = depth;
            } else if (*leaf_depth != depth) {
                in.setstate(std::ios::failbit);
                return nullptr;
            }
        } else {
            for (std::size_t i = 0; i <= static_cast<std::size_t>(count); ++i) {
                const Entry* child_lower = (i == 0) ? lower : &node->keys[i - 1];
                const Entry* child_upper = (i == count) ? upper : &node->keys[i];
                auto child = read_node(in, false, child_lower, child_upper, depth + 1, leaf_depth);
                if (!child) {
                    return nullptr;
                }
                node->children.push_back(std::move(child));
            }
        }
        return node;
    }

    std::unique_ptr<Node> root_;


public:

    BTree() = default;

    const Entry* find(const Entry& target) const {
        const Node* node = root_.get();
        while (node != nullptr) {
            const std::size_t i = key_index(*node, target);
            if ((i < node->keys.size()) && (node->keys[i] == target)) {
                return &(node->keys[i]);
            }
            if (node->children.empty()) {
                return nullptr;
            }
            node = node->children[i].get();
        }
        return nullptr;
    }

    bool insert(Entry entry) {
        if (find(entry) != nullptr) {
            return false;
        }
        if (root_ == nullptr) {
            root_ = std::make_unique<Node>();
            root_->keys.push_back(std::move(entry));
            return true;
        }
        if (root_->keys.size() == max_keys) {
            auto new_root = std::make_unique<Node>();
            new_root->children.push_back(std::move(root_));
            split_child(*new_root, 0);
            root_ = std::move(new_root);
        }
        insert_non_full(*root_, std::move(entry));
        return true;
    }

    bool erase(const Entry& target) {
        Entry key = target;
        if (find(key) == nullptr) {
            return false;
        }
        erase_from(*root_, key);
        if (root_->keys.empty()) {
            if (root_->children.empty()) {
                root_.reset();
            } else {
                auto new_root = std::move(root_->children[0]);
                root_ = std::move(new_root);
            }
        }
        return true;
    }

    bool save(const std::string& path) const {
        std::ofstream out(path, std::ios::binary);
        if (!out) return false;
        out.write("BTREE001", 8);
        const std::uint8_t present = static_cast<std::uint8_t>(root_ != nullptr);
        out.write(reinterpret_cast<const char*>(&present), sizeof(present));
        if (root_) write_node(out, *root_);
        out.close();
        return static_cast<bool>(out);
    }

    bool load(const std::string& path) {
        std::ifstream in (path, std::ios::binary);
        if (!in) {
            std::error_code ec;
            if ((!std::filesystem::exists(path, ec)) && (!ec)) {
                root_.reset();
                return true;
            }
            return false;
        }
        if (in.peek() == std::char_traits<char>::eof()) {
            if (!in.eof() || in.bad()) {
                return false;
            }
            root_.reset();
            return true;
        }
        char magic[8]{};
        in.read(magic, sizeof(magic));
        if ((!in) || (std::memcmp(magic, "BTREE001", 8) != 0)) {
            return false;
        }
        std::uint8_t present = 0;
        in.read(reinterpret_cast<char*>(&present), sizeof(present));
        if ((!in) || (present > 1)) return false;
        std::unique_ptr<Node> candidate;
        std::optional<std::size_t> leaf_depth;
        if (present == 1) {
            candidate = read_node(in, true, nullptr, nullptr, 0, leaf_depth);
            if (!candidate) {
                return false;
            }
        }
        if ((in.peek() != std::char_traits<char>::eof()) || in.bad()) {
            return false;
        }
        root_ = std::move(candidate);
        return true;        
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    BTree tree;
    std::string command;
    try {
        while (std::cin >> command) {
            if (command == "+") {
                std::string word;
                std::uint64_t value;
                if (!(std::cin >> word >> value)) break;
                if (tree.insert(Entry{std::move(word), value})) {
                    std::cout << "OK\n";
                } else {
                    std::cout << "Exist\n";
                }
            } else if (command == "-") {
                std::string word;
                if (!(std::cin >> word)) break;
                if (tree.erase(Entry{std::move(word)})) {
                    std::cout << "OK\n";
                } else {
                    std::cout << "NoSuchWord\n";
                }
            } else if (command == "!") {
                std::string operation, path;
                if (!(std::cin >> operation >> path)) {
                    break;
                }
                if (operation == "Save") {
                    if (tree.save(path)) {
                        std::cout << "OK\n";
                    } else {
                        std::cout << "ERROR: Cannot save file\n";
                    }
                } else if (operation == "Load") {
                    if (tree.load(path)) {
                        std::cout << "OK\n";
                    } else {
                        std::cout << "ERROR: Cannot load file\n";
                    }
                }
            } else {
                const Entry* found = tree.find(Entry{std::move(command)});
                if (found != nullptr) {
                    std::cout << "OK: " << found->value() << '\n';
                } else {
                    std::cout << "NoSuchWord\n";
                }
            }
        }
    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
    }
    return 0;
}