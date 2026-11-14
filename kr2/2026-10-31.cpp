#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

std::vector<int> z_calc(const std::string& s) {
    std::vector<int> z(size(s), 0);
    int L = -1, R = -1;
    for (int i = 0; i < static_cast<int>(size(s)); i++) {
        if (i <= R) {
            z[i] = std::min(R - i + 1, z[i - L]);
        }
        while (s[z[i]] == s[i + z[i]]) {
            z[i]++;
        }
        int new_R = i + z[i] - 1;
        if (new_R > R) {
            R = new_R;
            L = i;
        }
    }
    return z;
}

bool check_z(const std::string& s, const std::vector<int>& z) {
    int n = static_cast<int>(size(s));
    for (int i = 1; i < n; i++) {
        
    }
}