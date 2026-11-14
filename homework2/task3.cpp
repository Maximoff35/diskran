#include <iostream>
#include <vector>
#include <string>

class SegmentTree {
private:
    int n;
    std::vector<int> tree;

    void build(int v, int L, int R) {
        if (L == R) {
            tree[v] = 1;
            return;
        }
        int mid = (L + R) / 2;
        build(v * 2, L, mid);
        build(v * 2 + 1, mid + 1, R);
        tree[v] = tree[v * 2] + tree[v * 2 + 1];
    }

    void update(int v, int L, int R, int pos, int value) {
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

        tree[v] = tree[v * 2] + tree[v * 2 + 1];
    }

    int kth(int v, int L, int R, int k) {
        if (L == R) {
            return L;
        }
        int mid = (L + R) / 2;
        int left_count = tree[v * 2];
        if (k <= left_count) {
            return kth(v * 2, L, mid, k);
        } else {
            return kth(v * 2 + 1, mid + 1, R, k - left_count);
        }
    }

public:
    explicit SegmentTree(int n) 
        : n(n), tree(4 * n, 0) {
        build(1, 1, n);
    }
    int sell(int k) {
        int pos = kth(1, 1, n, k);
        update(1, 1, n, pos, 0);
        return pos;
    }
    void return_seat(int i) {
        update(1, 1, n, i, 1);
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    SegmentTree tree(n);

    std::string name;
    int x;

    for (int i = 0; i < q; ++i) {
        std::cin >> name >> x;
        if (name == "sell") {
            std::cout << tree.sell(x) << std::endl;
        } else {
            tree.return_seat(x);
        }
    }
}