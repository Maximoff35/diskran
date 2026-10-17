
#include <iostream>
#include <string>
#include "bst.hpp"

int main() {
    BST<int, std::string> tree;

    tree.add(10, "ten");
    tree.add(5, "five");
    tree.add(15, "fifteen");
    tree.add(3, "three");
    tree.add(7, "seven");
    tree.add(12, "twelve");
    tree.add(20, "twenty");

    std::cout << "Initial tree:\n";
    tree.print(std::cout);

    std::cout << "\nSize: " << tree.size();
    std::cout << "\nHeight: " << tree.height();
    std::cout << "\nWidth: " << tree.width() << '\n';

    if (auto* value = tree.find(7)) {
        std::cout << "\nFound: " << *value << '\n';
    }

    tree.erase(10);

    std::cout << "\nAfter deleting 10:\n";
    tree.print(std::cout);

    std::cout << "\nSize: " << tree.size();
    std::cout << "\nHeight: " << tree.height();
    std::cout << "\nWidth: " << tree.width() << '\n';

    tree.clear();

    std::cout << "\nAfter clearing:\n";
    std::cout << "Size: " << tree.size() << '\n';

    return 0;
}
