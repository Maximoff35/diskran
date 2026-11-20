#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <algorithm>

int ACGT_index(char c) {
    if (c == 'A') return 0;
    if (c == 'G') return 1;
    if (c == 'C') return 2;
    if (c == 'T') return 3;
    return -1;
}

std::vector<int> prefix(const std::string& s) {
    int n = s.size();
    std::vector<int> pi(n, 0);
    for (int i = 1; i < n; ++i) {
        int j = pi[i - 1];
        while ((j > 0) && (s[i] != s[j])) {
            j = pi[j - 1];
        }
        if (s[i] == s[j]) {
            ++j;
        }
        pi[i] = j;
    }
    return pi;
}

std::vector<std::vector<int>> preprocessing(const std::string& pattern, std::vector<int>& pi) {
    int m = pattern.size();
    std::vector<std::vector<int>> res(m);
    std::vector<int> zero(4, 0);
    zero[ACGT_index(pattern[0])] = 1;
    for (int i = 0; i < m; ++i) {
        if (pi[i] == 0) {
            res[i] = zero;
        } else {
            res[i] = res[pi[i] - 1];
        }
        if (i < m - 1) {
            res[i][ACGT_index(pattern[i + 1])] = i + 2;
        }
    }
    return res;
}

int main() {
    std::string pattern, text;
    std::cin >> pattern >> text;
    int m = pattern.size();
    int n = text.size();
    std::vector<int> pi = prefix(pattern);
    std::vector<std::vector<int>> tablitsa = preprocessing(pattern, pi);

    // for (int i : pi) {
    //     std::cout << i << ' ';
    // }
    // std::cout << '\n';
    // for (std::vector<int>& line : tablitsa) {
    //     for (int i : line) {
    //         std::cout << i << ' ';
    //     }
    //     std::cout << '\n';
    // }
    // std::cout << '\n';

    std::vector<int> result;
    int k = 0;
    for (int i = 0; i < n; ++i) {
        if (k == 0) {
            if (text[i] == pattern[0]) {
                k = 1;
            } else {
                k = 0;
            }
        } else {
            k = tablitsa[k - 1][ACGT_index(text[i])];
        }
        if (k == m) {
            result.push_back(i - m + 1);
        }
    }
    std::cout << result.size() << '\n';
    for (int res : result) {
        std::cout << res << '\n';
    }
    return 0;
}
