#include <iostream>
#include <vector>

long long merge_sort(std::vector<int>& a, std::vector<int>& temp, size_t L, size_t R) {
    if (R - L <= 1) return 0;
    size_t mid = (R + L) / 2;
    long long count = 0;
    count += merge_sort(a, temp, L, mid);
    count += merge_sort(a, temp, mid, R);
    size_t i = L;
    size_t j = mid;
    size_t k = L;

    while ((i < mid) && (j < R)) {
        if (a[i] <= a[j]) {
            temp[k] = a[i];
            ++k;
            ++i;
        } else {
            count += mid - i;
            temp[k] = a[j];
            ++k;
            ++j;
        }
    }
    while (i < mid) {
        temp[k] = a[i];
        ++k;
        ++i;
    }
    while (j < R) {
        temp[k] = a[j];
        ++k;
        ++j;
    }
    for (size_t p = L; p < R; ++p) {
        a[p] = temp[p];
    }
    return count;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    size_t n;
    std::cin >> n;
    std::vector<int> a(n);
    std::vector<int> temp(n);
    for (size_t i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

    std::cout << merge_sort(a, temp, 0, n) << '\n';

    return 0;
}