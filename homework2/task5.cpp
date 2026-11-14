#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

class SegmentTree final {
private:
    struct Node {
        int len;
        int pref;
        int suff;
        int best;
        int lazy;
    };

    int n;
    std::vector<Node> tree;

    static Node merge(const Node& left, const Node& right) {
        return Node{
            left.len + right.len,
            left.pref == left.len ? left.len + right.pref : left.pref,
            right.suff == right.len ? right.len + left.suff : right.suff,
            std::max({left.best, right.best, left.suff + right.pref}),
            -1,
        };
    }
    void build(int v, int L, int R) {
        if (L == R) {
            tree[v] = Node{1, 0, 0, 0, -1};
            return;
        }
        int mid = (L + R) / 2;
        build(v * 2, L, mid);
        build(v * 2 + 1, mid + 1, R);
        tree[v] = merge(tree[2 * v], tree[2 * v + 1]);
    }
    void apply(int v, int value) {
        if (value == 1) {
            tree[v].pref = tree[v].suff = tree[v].best = tree[v].len;
        } else {
            tree[v].pref = tree[v].suff = tree[v].best = 0;
        }
        tree[v].lazy = value;
    }
    void push(int v) {
        if (tree[v].lazy == -1) return;
        apply(v * 2, tree[v].lazy);
        apply(v * 2 + 1, tree[v].lazy);
        tree[v].lazy = -1;
    }
    void update(int v, int L, int R, int qL, int qR, int value) {
        if ((R < qL) || (L > qR)) return;
        if ((qL <= L) && (qR >= R)) {
            apply(v, value);
            return;
        }
        push(v);
        int mid = (L + R) / 2;
        update(v * 2, L, mid, qL, qR, value);
        update(v * 2 + 1, mid + 1, R, qL, qR, value);
        tree[v] = merge(tree[v * 2], tree[v * 2 + 1]);
    }
    Node query(int v, int L, int R, int qL, int qR) {
        if ((R < qL) || (L > qR)) {
            return Node{0, 0, 0, 0, -1};
        }
        if ((qL <= L) && (R <= qR)) {
            return tree[v];
        }
        push(v);
        int mid = (L + R) / 2;
        Node left = query(v * 2, L, mid, qL, qR);
        Node right = query(v * 2 + 1, mid + 1, R, qL, qR);
        return merge(left, right);
    }

public:
    explicit SegmentTree(int n) : n(n), tree(4 * n) {
        build(1, 1, n);
    }
    void on(int L, int R) {
        update(1, 1, n, L, R, 1);
    }
    void off(int L, int R) {
        update(1, 1, n, L, R, 0);
    }
    int max(int L, int R) {
        return query(1, 1, n, L, R).best;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;

    SegmentTree tree(n);
    std::string name;
    int L, R;
    for (size_t i = 0; i < q; ++i) {
        std::cin >> name >> L >> R;
        if (name == "on") {
            tree.on(L, R);
        } else if (name == "off") {
            tree.off(L, R);
        } else {
            std::cout << tree.max(L, R) << std::endl;
        }
    }
    return 0;
}