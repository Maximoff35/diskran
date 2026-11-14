#include <iostream>
#include <string>
#include <algorithm>

class AVLT final {
private:
    struct Node {
        int key;
        Node* left;
        Node* right;
        int value;
        int height;
        int size;
    };
    Node* root = nullptr;
    
    static int node_size(const Node* node);
    static int node_height(const Node* node);
    static void node_update_height_and_size(Node* node);
    static void node_destroy(Node* node);
    static Node* rotate_right(Node* node);
    static Node* rotate_left(Node* node);
    static Node* balance(Node* node);
    static Node* node_insert(Node* node, int key);
    static int node_count(const Node* node, int key);

public:
    ~AVLT();
    void event(int key);
    int count(int key);
};

int AVLT::node_size(const Node* node) {
    if (node == nullptr) return 0;
    return node->size;
}

int AVLT::node_height(const Node* node) {
    if (node == nullptr) return 0;
    return node->height;
}

void AVLT::node_update_height_and_size(Node* node) {
    node->height = 1 + std::max(node_height(node->left), node_height(node->right));
    node->size = node->value + node_size(node->left) + node_size(node->right);
}

auto AVLT::node_insert(Node* node, int key) -> Node* {
    if (node == nullptr) {
        return new Node{key, nullptr, nullptr, 1, 1, 1};
    }
    if (key < node->key) {
        node->left = node_insert(node->left, key);
    } else if (key > node->key) {
        node->right = node_insert(node->right, key);
    } else {
        node->value++;
    }
    return balance(node);
}

void AVLT::event(int key) {
    root = node_insert(root, key);
}

void AVLT::node_destroy(Node* node) {
    if (node == nullptr) return;
    node_destroy(node->right);
    node_destroy(node->left);
    delete node;
}

AVLT::~AVLT() {
    node_destroy(root);
    root = nullptr;
}

auto AVLT::rotate_left(Node* node) -> Node* {
    Node* new_root = node->right;
    node->right = new_root->left;
    new_root->left = node;
    node_update_height_and_size(node);
    node_update_height_and_size(new_root);
    return new_root;
}

auto AVLT::rotate_right(Node* node) -> Node* {
    Node* new_root = node->left;
    node->left = new_root->right;
    new_root->right = node;
    node_update_height_and_size(node);
    node_update_height_and_size(new_root);
    return new_root;
}

auto AVLT::balance(Node* node) -> Node* {
    node_update_height_and_size(node);
    int bf = node_height(node->right) - node_height(node->left);
    if (bf == 2) {
        if (node_height(node->right->left) > node_height(node->right->right)) {
            node->right = rotate_right(node->right);
        }
        return rotate_left(node);
    }
    if (bf == -2) {
        if (node_height(node->left->right) > node_height(node->left->left)) {
            node->left = rotate_left(node->left);
        }
        return rotate_right(node);
    }
    return node;
}

int AVLT::node_count(const Node* node, int key) {
    if (node == nullptr) return 0;
    if (key < node->key) return node_count(node->left, key);
    else if (key == node->key) return node_size(node->left) + node->value;
    else return node_size(node->left) + node->value + node_count(node->right, key);
}

int AVLT::count(int key) {
    return node_count(root, key);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t q;
    std::cin >> q;

    AVLT tree;
    std::string command;
    int key, key2;

    for (size_t i = 0; i < q; ++i) {
        std::cin >> command >> key;
        if (command == "count") {
            std::cin >> key2;
            std::cout << tree.count(key) - tree.count(key - key2) << std::endl;
        } else {
            tree.event(key);
        }
    }

    return 0;
}