#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <utility>
#include <sstream>
#include <cstddef>

struct Line {
    int day;
    int month;
    int year;
    std::string line;
};

template<std::size_t K, typename KeyFunc>
void counting_sort(
    std::vector<Line>& a,
    std::vector<Line>& tmp,
    KeyFunc key
) {
    std::array<int, K> count{};
    for (const Line& item : a) {
        count[key(item)]++;
    }
    for (int i = 1; i < K; i++) {
        count[i] += count[i - 1];
    }
    int n = static_cast<int>(a.size());
    for (int i = n - 1; i >= 0; i--) {
        int k = key(a[i]);
        count[k]--;
        tmp[count[k]] = std::move(a[i]);
    }
    a.swap(tmp);
}

int main() {
    std::vector<Line> a;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }

        std::istringstream ss(line);
        int day, month, year;
        char dot1, dot2;
        if (!(ss >> day >> dot1 >> month >> dot2 >> year) || (dot1 != '.') || (dot2 != '.')) {
            continue;
        }
        a.push_back({day, month, year, line});
    }
    std::vector<Line> tmp(a.size());

    // passes: D, M, Y;
    counting_sort<32>(a, tmp, [](const Line& x) {
        return x.day;
    });
    counting_sort<13>(a, tmp, [](const Line& x) {
        return x.month;
    });
    for (int exp = 1; exp <= 1000; exp *= 10) {
        counting_sort<10>(a, tmp, [exp](const Line& x) {
            return (x.year / exp) % 10;
        });
    }

    for (const Line& item : a) {
        std::cout << item.line << '\n';
    }
}