#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

struct Bucket {
    long double min = 0;
    long double max = 0;
    bool used = false;
};

int main() {
    int n;
    std::cin >> n;
    std::vector<long double> a(n);
    long double L = 1e9;
    long double R = -1e9;
    for (int i = 0; i < n; i++) {
        std::cin >> a[i];
        L = std::min(L, a[i]);
        R = std::max(R, a[i]);
    }
    if ((n < 2) || (L == R)) {
        std::cout << 0 << '\n';
        return 0;
    }
    std::vector<Bucket> b(n);
    for (int i = 0; i<n; i++) {
        int ind = static_cast<int>((a[i] - L) * (n - 1) / (R - L));
        if (!b[ind].used) {
            b[ind].min = b[ind].max = a[i];
            b[ind].used = true;
        } else {
            b[ind].min = std::min(b[ind].min, a[i]);
            b[ind].max = std::max(b[ind].max, a[i]);
        }
    }
    long double res = 0;
    long double prev_max = L;
    for (int i = 0; i < n; i++) {
        if (!b[i].used) {
            continue;
        }
        res = std::max(res, b[i].min - prev_max);
        res = std::max(res, b[i].max - b[i].min);
        prev_max = b[i].max;
    }
    std::cout << std::setprecision(15) << res << '\n';
    return 0;
}