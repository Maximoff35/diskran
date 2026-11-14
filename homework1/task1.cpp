#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <array>


void counting_sort(std::vector<std::pair<std::string, int>>& a, int n) {
    std::array<int, 121> count{};
    for (int i = 0; i < n; i++) {
        count[a[i].second]++;
    }
    for (int i = 1; i < 121; i++) {
        count[i] += count[i - 1];
    }
    std::vector<std::pair<std::string, int>> b(n);
    for (int i = n - 1; i >= 0; i--) {
        count[a[i].second] -= 1;
        b[count[a[i].second]] = a[i];
    }
    a = std::move(b);
}


int main() {
    int n;
    std::cin >> n;
    std::cin.ignore();
    
    std::vector<std::pair<std::string, int>> a(n);
    
    for (int i = 0; i < n; i++) {
        std::getline(std::cin, a[i].first);
        std::istringstream ss(a[i].first);
        std::string name;
        ss >> name >> a[i].second;
    }

    counting_sort(a, n);

    for (int i = 0; i < n; i++) {
        std::cout << a[i].first << '\n';
    }

    return 0;
}