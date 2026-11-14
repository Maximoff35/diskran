#pragma once

#include <concepts>
#include <vector>
#include <cstddef>
#include <memory>
#include <utility>

template<typename T>
concept Comparable = requires(const T& a, const T& b) {
    { a < b } -> std::convertible_to<bool>;
    { a > b } -> std::convertible_to<bool>;
    { a == b } -> std::convertible_to<bool>;
};

template<Comparable Key, typename Value>
class BTree final {
private:
    using Entry = std::pair<Key, Value>;

    struct Node {
        bool leaf;
        std::vector<Entry> entries;
        std::vector<std::unique_ptr<Node>> children;

        explicit Node(bool is_leaf);
    };

    std::unique_ptr<Node> root_;
    std::size_t t_;
    std::size_t size_ = 0;

public:
    explicit BTree(std::size_t t);

    bool insert(const Key& key, const Value& value);
    bool erase(const Key& key);

    Value* find(const Key& key);
    const Value* find(const Key& key) const;

    bool contains(const Key& key) const;

    std::size_t size() const noexcept;
    bool empty() const noexcept;
    void clear() noexcept;

private:
    std::size_t find_index(const Node& node, const Key& key) const;

    void split_child(Node& parent, std::size_t index);
    void insert_non_full(Node& node, const Entry& entry);

    void erase_from_node(Node& node, const Key& key);
    void erase_from_internal(Node& node, std::size_t index);

    Entry predecessor(const Node& node) const;
    Entry successor(const Node& node) const;

    std::size_t prepare_child(Node& parent, std::size_t index);

    void borrow_from_prev(Node& parent, std::size_t index);
    void borrow_from_next(Node& parent, std::size_t index);
    void merge_children(Node& parent, std::size_t index);
};