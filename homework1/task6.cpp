#include <iostream>

bool check(unsigned long long x, unsigned long long a, int k) {
    if (x == 0) return true;
    unsigned long long res = 1;
    for (int i = 0; i < k; i++) {
        if (res > a / x) {
            return false;
        }
        res *= x;
    }
    return true;
}

unsigned long long bin_find(unsigned long long a, int k) {
    unsigned long long l = 0;
    unsigned long long r = 1ULL << 32;
    unsigned long long mid;
    while (r - l > 1) {
        mid = (l + r) / 2;
        if (check(mid, a, k)) {
            l = mid;
        } else {
            r = mid;
        }
    }
    return l;
}

int main() {
    unsigned long long a;
    int k;
    std::cin >> a >> k;
    std::cout << bin_find(a, k) << '\n';
    return 0;
}