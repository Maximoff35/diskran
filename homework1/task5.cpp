#include <iostream>
#include <string>
#include <algorithm>

class BST {
private:
    struct Node {
        int key;
        Node* left;
        Node* right;
        int size;
        int height;
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
    static Node* node_find_min(Node* node);
    static Node* node_remove(Node* node, int key);
    static int node_kth(const Node* node, int k);
    static int node_less(const Node* node, int key);
public:
    ~BST();
    void insert(int key);
    void remove(int key);
    int kth(int k) const;
    int less(int key) const;
};

void BST::node_destroy(Node* node) {
    if (node == nullptr) return;
    node_destroy(node->left);
    node_destroy(node->right);
    delete node;
}

BST::~BST() {
    node_destroy(root);
    root = nullptr;
}

int BST::node_size(const Node* node) {
    if (node == nullptr) return 0;
    return node->size;
}

int BST::node_height(const Node* node) {
    if (node == nullptr) return 0;
    return node->height;
}

void BST::node_update_height_and_size(Node* node) {
    node->height = 1 + std::max(node_height(node->left), node_height(node->right));
    node->size = 1 + node_size(node->left) + node_size(node->right);
}

auto BST::rotate_right(Node* node) -> Node* {
    Node* new_root = node->left;
    node->left = new_root->right;
    new_root->right = node;
    node_update_height_and_size(node);
    node_update_height_and_size(new_root);
    return new_root;
}

auto BST::rotate_left(Node* node) -> Node* {
    Node* new_root = node->right;
    node->right = new_root->left;
    new_root->left = node;
    node_update_height_and_size(node);
    node_update_height_and_size(new_root);
    return new_root;
}

auto BST::balance(Node* node) -> Node* {
    node_update_height_and_size(node);
    int balance_factor = node_height(node->right) - node_height(node->left);
    if (balance_factor == 2) {
        if (node_height(node->right->left) > node_height(node->right->right)) {
            node->right = rotate_right(node->right);
        }
        return rotate_left(node);
    }
    if (balance_factor == -2) {
        if (node_height(node->left->right) > node_height(node->left->left)) {
            node->left = rotate_left(node->left);
        }
        return rotate_right(node);
    }
    return node;
}

auto BST::node_insert(Node* node, int key) -> Node* {
    if (node == nullptr) {
        return new Node{key, nullptr, nullptr, 1, 1};
    }
    if (key < node->key) {
        node->left = node_insert(node->left, key);
    } else if (key > node->key) {
        node->right = node_insert(node->right, key);
    }
    return balance(node);
}

void BST::insert(int key) {
    root = node_insert(root, key);
}

auto BST::node_find_min(Node* node) -> Node* {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

auto BST::node_remove(Node* node, int key) -> Node* {
    if (node == nullptr) return nullptr;
    if (key < node->key) {
        node->left = node_remove(node->left, key);
    } else if (key > node->key) {
        node->right = node_remove(node->right, key);
    } else {
        if (node->left == nullptr) {
            Node* child = node->right;
            delete node;
            return child;
        } else if (node->right == nullptr) {
            Node* child = node->left;
            delete node;
            return child;
        } else {
            int successor = node_find_min(node->right)->key;
            node->key = successor;
            node->right = node_remove(node->right, successor);
        }
    }
    return balance(node);
}

void BST::remove(int key) {
    root = node_remove(root, key);
}

int BST::node_kth(const Node* node, int k) {
    int left_size = node_size(node->left);
    if (k == left_size + 1) return node->key;
    else if (k < left_size + 1) return node_kth(node->left, k);
    else return node_kth(node->right, k - left_size - 1);
}

int BST::kth(int k) const {
    return node_kth(root, k);
}

int BST::node_less(const Node* node, int key) {
    if (node == nullptr) return 0;
    if (node->key == key) return node_size(node->left);
    else if (node->key > key) return node_less(node->left, key);
    else return 1 + node_size(node->left) + node_less(node->right, key);
}

int BST::less(int key) const {
    return node_less(root, key);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int m;
    std::cin >> m;
    std::string name;
    int value;
    BST tree;
    for (int i = 0; i < m; i++) {
        std::cin >> name >> value;
        if (name == "insert") {
            tree.insert(value);
        } else if (name == "delete") {
            tree.remove(value);
        } else if (name == "kth") {
            std::cout << tree.kth(value) << '\n';
        } else if (name == "less") {
            std::cout << tree.less(value) << '\n';
        }
    }
    return 0;
}