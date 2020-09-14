#include <iostream>
#include <vector>
#include <set>
#include <limits>
#include <algorithm>

using namespace std;

size_t pow2(size_t num) {
    size_t k = 1;
    while (k < num) {
        k <<= 1;
    }
    return k;
}

class SegTree {
private:
    const vector<long> &_arr;
    vector<multiset<long>> tree;
    size_t len2;

    size_t left(size_t v) {
        return 2 * v + 1;
    }

    size_t right(size_t v) {
        return 2 * v + 2;
    }

    size_t parent(size_t v) {
        return (v - 1) / 2;
    }

    void _update(size_t v, long x) {
        long old = *tree[v].begin();
        do {
            auto iter = tree[v].find(old);
            if (iter != tree[v].end()) {
                tree[v].erase(iter);
            }
            tree[v].insert(x);
            v = parent(v);
        } while(v != 0);
    }

    long _compute(size_t v, long x, size_t ql, size_t qr, size_t lb, size_t rb) {
        if (rb <= ql or qr <= lb) {
            return numeric_limits<long>::max();
        }
        if (ql <= lb and qr >= rb) {
            auto iter = tree[v].upper_bound(x);
            if (iter != tree[v].end()) {
                return *iter;
            } else {
                return numeric_limits<long>::max();
            }
        }
        size_t sm = (rb + lb) / 2;
        return min(_compute(left(v), x, ql, qr, lb, sm),
                   _compute(right(v), x, ql, qr, sm, rb));
    }

public:
    SegTree(const vector<long> &arr)
            : _arr(arr)
            , len2(pow2(arr.size()))
            , tree(pow2(arr.size()) * 2 - 1) {
        for (size_t i = 0; i < arr.size(); ++i) {
            tree[i + len2 - 1] = {arr[i]};
        }
        for (long i = len2 - 2; i >= 0; --i) {
            merge(tree[left(i)].begin(), tree[left(i)].end(),
                  tree[right(i)].begin(), tree[right(i)].end(),
                  inserter(tree[i], tree[i].end()));
        }
    }

    long compute(size_t ql, size_t qr, long x) {
        return _compute(0, x, ql, qr, 0, len2);
    }

    void update(size_t i, long x) {
        _update(i + len2 - 1, x);
    }
};

int main() {
    long q, t, l, r, x;
    size_t n;
    cin >> n;
    vector<long> arr(n);
    for (auto &i : arr) {
        cin >> i;
    }

    SegTree tree(arr);

    cin >> q;
    for (int i = 0; i < q; ++i) {
        cin >> t;
        if (t == 1) {
        cin >> l >> r >> x;
        x = tree.compute(l, r + 1, x);
        if (x == numeric_limits<long>::max()) {
            cout << "NONE" << '\n';
        } else {
            cout << x << '\n';
        }
        } else if (t == 2) {
            cin >> l >> x;
            tree.update(l, x);
        }
    }
    return 0;
}