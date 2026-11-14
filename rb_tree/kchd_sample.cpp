#include <concepts>
#include <cassert>

template<typename T>
concept Comparable = requires(const T& a, const T& b) {
    { a < b } -> std::convertible_to<bool>;
    { a == b } -> std::convertible_to<bool>;
    { a <= b } -> std::convertible_to<bool>;
};

template<Comparable Key, typename Value>
class Tree {
public:
    Tree() {
        null_node = create_node(Node());
        null_node->color = Color::Black;
    }

    int insert(const Key& key, const Value& value) {
        Node* parent = find_parent(key);
        Node* new_node = create_node(Node(key, value), parent);
        if (parent == nullptr) {
            root = new_node;
            root->color = Color::Black;
            return;
        }
        if (key <= parent->key) {
            parent->l = new_node;
        } else {
            parent->r = new_node;
        }
        new_node->parent = parent;
        new_node->insert_node();
        while (root->parent != nullptr) {
            root = root->parent;
        }
        root->color = Color::Black;
    }

private:
    enum class Color {
        Red,
        Black
    };

    class Node {
    public:
        Key key;
        Value value;

        Node* l = nullptr;
        Node* r = nullptr;
        Node* parent = nullptr;

        Color color = Color::Red;

        Node() {}
        Node(const Key& key, const Value& value) : key(key), value(value) {}

        static Color get_color(Node* n) {
            if (n == nullptr || n->color == Color::Black) {
                return Color::Black;
            } else {
                return Color::Red;
            }
        }

        Node* get_bro() {
            if (parent == nullptr) {
                return nullptr;
            }
            if (parent->l == this) {
                return parent->r;
            } else {
                return parent->l;
            }
        }

        void node_insert() {
            if ((get_color(parent) == Color::Black) || (parent->parent == nullptr)) {
                return;
            }
            Node* uncle = parent->get_bro();
            if (get_color(uncle) == Color::Red) {
                parent->parent->swap_colors();
                parent->parent->node_insert();
                return;
            }

            // отец левый сын
            if (parent->parent->l == parent) {
                if (parent->r == this) {
                    parent->left_rotate();
                    l->node_insert();
                    return;
                }
                parent->parent->right_rotate();
                
            } else {
                if (parent->l == this) {
                    parent->right_rotate();
                    r->node_insert();
                    return;
                }
                parent->parent->left_rotate();
                
            }
            parent->color = Color::Black;
            Node* bro = get_bro();
            bro->color = Color::Red;

            
        }

        void replace_child(Node* old_child, Node* new_child) {
            if (old_child == l) {
                l = new_child;
            } else {
                r = new_child;
            }
        }

        bool is_null_node() {
            return l == nullptr && r == nullptr;
        }

        void right_rotate() {
            Node* y = l;
            l = y->r;
            if (!l->is_null_node()) {
                l->parent = y;
            }
            y->r = this;
            y->parent = parent;

            bool is_root = true;
            if (parent != nullptr) {
                parent->replace_child(this, y);
                is_root = false;
            }

            parent = y;

            Node* res = nullptr;
            if (is_root) {
                res = y;
            }
            return res;
        }

        Node* left_rotate() {
            Node* y = r;
            r = y->l;
            if (!r->is_null_node()) {
                r->parent = y;
            }
            y->l = this;
            y->parent = parent;

            bool is_root = true;
            if (parent != nullptr) {
                parent->replace_child(this, y);
                is_root = false;
            }

            parent = y;

            Node* res = nullptr;
            if (is_root) {
                res = y;
            }
            return res;
        }

        void swap_colors() {
            color = !color;
            l->color = !l->color;
            r->color = !r->color;
        }

        Node* find_parent(const Key& target_key) {
            if (target_key <= key) {
                if (l->is_null_node()) {
                    return this;
                }
                return l->find_parent(target_key);
            } else {
                if (r->is_null_node()) {
                    return this;
                }
                return r->find_parent(target_key);
            }
        }
    };

    Node* root = nullptr;
    Node* null_node = nullptr;

    Node* create_node(const Node& node, Node* parent) {
        Node* new_node = new Node(node);
        new_node->l = null_node;
        new_node->r = null_node;
        new_node->parent = parent;
    }

    Node* find_parent(const Key& key) {
        if (root == nullptr) {
            return nullptr;
        }

        root->find_parent(key);
    }
};
