#include <utility>
#include <queue>
#include <ostream>

template<Comparable Key, typename Value>
BST<Key, Value>::Node::Node(Key k, Value v)
    : key(std::move(k)), value(std::move(v)) {}

template<Comparable Key, typename Value>
bool BST<Key, Value>::node_add(Node*& place, Key& key, Value& value) {
    if (place == nullptr) {
        place = new Node(std::move(key), std::move(value));
        return true;
    }
    if (key < place->key) {
        return node_add(place->left, key, value);
    } else if (key > place->key) {
        return node_add(place->right, key, value);
    }
    return false;
}

template<Comparable Key, typename Value>
bool BST<Key, Value>::add(Key key, Value value) {
    return node_add(root, key, value);
}

template<Comparable Key, typename Value>
void BST<Key, Value>::node_destroy(Node* node) {
    if (node == nullptr) return;
    node_destroy(node->left);
    node_destroy(node->right);
    delete node;
}

template<Comparable Key, typename Value>
void BST<Key, Value>::clear() {
    node_destroy(root);
    root = nullptr;
}

template<Comparable Key, typename Value>
BST<Key, Value>::~BST() {
    clear();
}

template<Comparable Key, typename Value>
auto BST<Key, Value>::node_find(Node*& place, const Key& key) -> Node*& {
    if (place == nullptr) return place;
    if (key == place->key) return place;
    else if (key < place->key) return node_find(place->left, key);
    else return node_find(place->right, key);
}

template<Comparable Key, typename Value>
auto BST<Key, Value>::node_find(const Node* node, const Key& key) -> const Node* {
    if (node == nullptr) return nullptr;
    if (key == node->key) return node;
    else if (key < node->key) return node_find(node->left, key);
    else return node_find(node->right, key);
}

template<Comparable Key, typename Value>
Value* BST<Key, Value>::find(const Key& key) {
    Node*& place = node_find(root, key);
    if (place == nullptr) return nullptr;
    return &(place->value);
}

template<Comparable Key, typename Value>
const Value* BST<Key, Value>::find(const Key& key) const {
    const Node* node = node_find(root, key);
    if (node == nullptr) return nullptr;
    return &(node->value);
}

template<Comparable Key, typename Value>
auto BST<Key, Value>::node_find_min(Node*& place) -> Node*& {
    if ((place == nullptr) || (place->left == nullptr)) return place;
    else return node_find_min(place->left);
}

template<Comparable Key, typename Value>
void BST<Key, Value>::node_delete(Node*& place) {
    if (place == nullptr) return;
    Node* old = place;
    if (old->left == nullptr) {
        place = old->right;
    } else if (old->right == nullptr) {
        place = old->left;
    } else {
        Node*& min_place = node_find_min(old->right);
        Node* replacement = min_place;
        min_place = replacement->right;
        replacement->left = old->left;
        replacement->right = old->right;
        place = replacement;
    }
    delete old;
}

template<Comparable Key, typename Value>
bool BST<Key, Value>::erase(const Key& key) {
    Node*& place = node_find(root, key);
    if (place == nullptr) return false;
    node_delete(place);
    return true;
}

template<Comparable Key, typename Value>
std::size_t BST<Key, Value>::node_size(const Node* node) {
    if (node == nullptr) return 0;
    else return 1 + node_size(node->left) + node_size(node->right);
}

template<Comparable Key, typename Value>
std::size_t BST<Key, Value>::size() const {
    return node_size(root);
}

template<Comparable Key, typename Value>
std::size_t BST<Key, Value>::node_height(const Node* node) {
    if (node == nullptr) return 0;
    std::size_t left_height = node_height(node->left);
    std::size_t right_height = node_height(node->right);
    return 1 + (left_height > right_height ? left_height : right_height);
}

template<Comparable Key, typename Value>
std::size_t BST<Key, Value>::height() const {
    return node_height(root);
}

template<Comparable Key, typename Value>
std::size_t BST<Key, Value>::node_width(const Node* node) {
    if (node == nullptr) return 0;
    std::queue<const Node*> q;
    q.push(node);
    std::size_t max_width = 0;
    while (!q.empty()) {
        std::size_t level_size = q.size();
        if (level_size > max_width) {
            max_width = level_size;
        }
        for (std::size_t i = 0; i < level_size; i++) {
            const Node* current = q.front();
            q.pop();
            if (current->left != nullptr) {
                q.push(current->left);
            }
            if (current->right != nullptr) {
                q.push(current->right);
            }
        }
    }
    return max_width;
}

template<Comparable Key, typename Value>
std::size_t BST<Key, Value>::width() const {
    return node_width(root);
}

template<Comparable Key, typename Value>
void BST<Key, Value>::node_print(const Node* node, std::ostream& out, std::size_t depth) {
    if (node == nullptr) return;
    node_print(node->right, out, depth + 1);
    for (std::size_t i = 0; i < depth; i++) {
        out << "    ";
    }
    out << node->key << '\n';
    node_print(node->left, out, depth + 1);
}

template<Comparable Key, typename Value>
void BST<Key, Value>::print(std::ostream& out) const {
    node_print(root, out, 0);
}

template<Comparable Key, typename Value>
BST<Key, Value>::BST(BST&& other) noexcept 
    : root(other.root) {
        other.root = nullptr;
}

template<Comparable Key, typename Value>
BST<Key, Value>& BST<Key, Value>::operator=(BST&& other) noexcept {
    if (this != &other) {
        clear();
        root = other.root;
        other.root = nullptr;
    }
    return *this;
}