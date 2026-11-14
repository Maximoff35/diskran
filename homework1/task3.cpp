#include <iostream>
#include <vector>


int bin_find(std::vector<long long>& a, int n, long long key) {
    int l = 0, r = n - 1;
    int mid;
    while (l <= r) {
        mid = (r + l) / 2;
        if (key <= a[mid]) {
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    return l;
}


int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, q;
    std::cin >> n >> q;
    std::vector<long long> a(n);
    for (int i = 0; i < n; i++) {
        std::cin >> a[i];
    }
    long long current;
    for (int i = 0; i < q; i++) {
        std::cin >> current;
        int first = bin_find(a, n, current);
        int second = -1;
        if (first < n) {
            second = bin_find(a, n, current + 1);
        } else {
            first = -1;
        }
        int count = second - first;
        if (count < 0) count = -1;
        if (second >= n) second = -1;
        std::cout << first << ' ' << second << ' ' << count << '\n';
    }
    return 0;
}