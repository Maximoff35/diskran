#include <concepts>

template<typename T>
concept Comparable = requires(const T& a, const T& b) {
    { a < b } -> std::convertible_to<bool>;
    { a == b } -> std::convertible_to<bool>;
};

template<Comparable Key, typename Value>
class Tree {
public:
    Tree() {
        null_node = create_node(Node());
    }

private:
    enum Color {
        RED,
        BLACK
    };
    class Node {
        Key key;
        Value value;
        Node* r = nullptr;
        Node* l = nullptr;
        Color color;
        Node() {};
    };

    Node* null_node = nullptr;
    Node* root = nullptr;

    Node* create_node(const Node& node) {
        return new Node(node);
    }
};