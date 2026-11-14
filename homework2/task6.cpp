#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

class SegmentTree final {
private:
    struct Node {
        long long sum;
        long long pref;
        long long suff;
        long long best;
    };
    int n;
    std::vector<long long> a;
    std::vector<Node> tree;
    static Node merge(Node left, Node right) {
        return Node{
            left.sum + right.sum,
            std::max(left.sum + right.pref, left.pref),
            std::max(right.sum + left.suff, right.suff),
            std::max({left.best, right.best, right.pref + left.suff})
        };
    }
    void build(int v, int L, int R) {
        if (L == R) {
            tree[v] = Node{a[L], a[L], a[L], a[L]};
            return;
        }
        int mid = (L + R) / 2;
        build(v * 2, L, mid);
        build(v * 2 + 1, mid + 1, R);
        tree[v] = merge(tree[2 * v], tree[2 * v + 1]);
    }
    void update_(int v, int L, int R, size_t pos, long long value) {
        if (L == R) {
            tree[v] = Node{value, value, value, value};
            return;
        }
        int mid = (L + R) / 2;
        if (pos <= mid) {
            update_(2 * v, L, mid, pos, value);
        } else {
            update_(2 * v + 1, mid + 1, R, pos, value);
        }
        tree[v] = merge(tree[2 * v], tree[2 * v + 1]);
    }
    Node query(int v, int L, int R, int qL, int qR) {
        if ((qL <= L) && (R <= qR)) {
            return tree[v];
        }
        int mid = (L + R) / 2;
        if (qR <= mid) {
            return query(2 * v, L, mid, qL, qR);
        }
        if (qL > mid) {
            return query(2 * v + 1, mid + 1, R, qL, qR);
        }
        Node left = query(2 * v, L, mid, qL, qR);
        Node right = query(2 * v + 1, mid + 1, R, qL, qR);
        return merge(left, right);
    }
public:
    explicit SegmentTree(int n, std::vector<long long>& a) 
        : n(n), a(a), tree(4 * n) {
            build(1, 1, n);
        }
    void update(size_t pos, long long value) {
        a[pos] = value;
        update_(1, 1, n, pos, value);
    }
    long long best(int L, int R) {
        return query(1, 1, n, L, R).best;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    std::vector<long long> a(n + 1);
    for (size_t i = 1; i <= n; ++i) {
        std::cin >> a[i];
    }
    SegmentTree tree(n, a);
    std::string name;
    int first, second;
    for (size_t i = 0; i < q; ++i) {
        std::cin >> name >> first >> second;
        if (name == "update") {
            tree.update(first, (long long)second);
        } else {
            std::cout << tree.best(first, second) << std::endl;
        }
    }

    return 0;
}