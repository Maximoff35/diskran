#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class SegmentTree final {
private:
    int n;
    std::vector<int> a;
    std::vector<int> tree;


    void build(int v, int L, int R) {
        if (L == R) {
            tree[v] = a[L];
            return;
        }
        int mid = (L + R) / 2;
        build(v * 2, L, mid);
        build(v * 2 + 1, mid + 1, R);

        tree[v] = std::min(tree[v * 2], tree[v * 2 + 1]);
    }

    void update(int v, int L, int R, int pos, int value) {
        a[pos] = value;
        if (L == R) {
            tree[v] = value;
            return;
        }
        int mid = (L + R) / 2;
        if (pos <= mid) {
            update(v * 2, L, mid, pos, value);
        } else {
            update(v * 2 + 1, mid + 1, R, pos, value);
        }
        tree[v] = std::min(tree[2 * v], tree[v * 2 + 1]);
    }

    int query(int v, int L, int R, int qL, int qR) {
        if ((R < qL) || (L > qR)) {
            return INT_MAX;
        }
        if ((qL <= L) && (R <= qR)) {
            return tree[v];
        }
        int mid = (L + R) / 2;
        return std::min(query(v * 2, L, mid, qL, qR), query(v * 2 + 1, mid + 1, R, qL, qR));
    }
public:
    explicit SegmentTree(int n, std::vector<int>& a)
        : n(n), a(a), tree(4 * n, 0) {
            build(1, 1, n);
        }
    void set(int pos, int value) {
        update(1, 1, n, pos, value);
    }
    int min(int L, int R) {
        return query(1, 1, n, L, R);
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;

    std::vector<int> a(n + 1);
    for (size_t i = 1; i <= n; ++i) {
        std::cin >> a[i];
    }
    SegmentTree tree(n, a);
    std::string name;
    int first, second;
    for (size_t i = 0; i < q; ++i) {
        std::cin >> name >> first >> second;
        if (name == "set") {
            tree.set(first, second);
        } else {
            std::cout << tree.min(first, second) << std::endl;
        }
    }
    return 0;
}