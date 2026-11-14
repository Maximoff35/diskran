#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

std::vector<int> z_calc(const std::string& s) {
    int n = static_cast<int>(s.size());
    std::vector<int> z(n, 0);
    int L = -1, R = -1;
    for (int i = 1; i < n; ++i) {
        if (i <= R) {
            z[i] = std::min(R - i + 1, z[i - L]);
        }
        while ((i + z[i] < n) && (s[z[i]] == s[i + z[i]])) {
            ++z[i];
        }
        int new_R = i + z[i] - 1;
        if (new_R > R) {
            R = new_R;
            L = i;
        }
    }
    return z;
}

std::vector<int> prefix(const std::string& s) {
    int n = static_cast<int>(s.size());
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

std::vector<int> strong_prefix(const std::string& s) {
    int n = static_cast<int>(s.size());
    std::vector<int> z = z_calc(s);
    std::vector<int> strong(n, 0);
    for (int j = 1; j < n; ++j) {
        if (z[j] == 0) {
            continue;
        }
        int i = j + z[j] - 1;
        strong[i] = std::max(strong[i], z[j]);
    }
    return strong;
}

std::vector<int> find_with_prefix(const std::string& text, const std::string& pattern) {
    std::string s = pattern + '#' + text;
    std::vector<int> pi = prefix(s);
    std::vector<int> positions;
    int m = static_cast<int>(pattern.size());
    for (int i = m + 1; i < static_cast<int>(s.size()); ++i) {
        if (pi[i] == m) {
            positions.push_back(i - 2 * m);
        }
    }
    return positions;
}

std::vector<int> kmp_search(const std::string& text, const std::string& pattern) {
    int n = static_cast<int>(text.size());
    int m = static_cast<int>(pattern.size());
    std::vector<int> strong = strong_prefix(pattern);
    std::vector<int> positions;
    int j = 0;
    for (int i = 0; i < n; ++i) {
        while ((j > 0) && (text[i] != pattern[j])) {
            j = strong[j - 1];
        }
        if (text[i] == pattern[j]) {
            ++j;
        }
        if (j == m) {
            positions.push_back(i - m + 1);
            j = strong[m - 1];
        }
    }
    return positions;
}