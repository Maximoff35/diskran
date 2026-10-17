#pragma once

#include <concepts>
#include <cstddef>
#include <iosfwd>

template<typename T>
concept Comparable = requires(const T& a, const T& b) {
    { a < b } -> std::convertible_to<bool>;
    { a > b } -> std::convertible_to<bool>;
    { a == b } -> std::convertible_to<bool>;
};

template<Comparable Key, typename Value>
class BST final {
private:
    struct Node {
        Key key;
        Value value;
        Node* left = nullptr;
        Node* right = nullptr;

        Node(Key k, Value v);
    };

    Node* root = nullptr;

    static bool node_add(Node*& place, Key& key, Value& value);
    static void node_destroy(Node* node);

    static Node*& node_find(Node*& place, const Key& key);
    static const Node* node_find(const Node* node, const Key& key);

    static Node*& node_find_min(Node*& place);
    static void node_delete(Node*& place);

    static std::size_t node_size(const Node* node);
    static std::size_t node_height(const Node* node);
    static std::size_t node_width(const Node* node);

    static void node_print(const Node* node, std::ostream& out, std::size_t depth);

public:
    BST() = default;
    ~BST();

    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;

    BST(BST&& other) noexcept;
    BST& operator=(BST&& other) noexcept;

    bool add(Key key, Value value);
    bool erase(const Key& key);

    Value* find(const Key& key);
    const Value* find(const Key& key) const;

    std::size_t size() const;
    std::size_t height() const;
    std::size_t width() const;

    void print(std::ostream& out) const;
    void clear();
};

#include "bst.tpp"