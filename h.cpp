#include <iostream>
#include <vector>
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
    vector<vector<long>> tree;
    size_t len2;

    size_t left(size_t v) {
        return 2 * v + 1;
    }

    size_t right(size_t v) {
        return 2 * v + 2;
    }
    /*
    void _update(size_t v, size_t i, long x, size_t lb, size_t rb) {
        if (!(lb <= i and i < rb)) {
            return;
        }
        if (v == i + len2 - 1) {
            tree[v] = node(x, x);
            return;
        }
        size_t sm = (lb + rb) / 2;
        _update(left(v), i, x, lb, sm);
        _update(right(v), i, x, sm, rb);
        tree[v].min = min(tree[left(v)].min, tree[right(v)].min);
        tree[v].max = max(tree[left(v)].max, tree[right(v)].max);
    } */

    long _compute(size_t v, long x, size_t ql, size_t qr, size_t lb, size_t rb) {
        if (rb <= ql or qr <= lb) {
            return numeric_limits<long>::max();
        }
        if (ql <= lb and qr >= rb) {
            auto iter = upper_bound(tree[v].begin(), tree[v].end(), x);
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
                  back_inserter(tree[i]));
        }
    }

    long compute(size_t ql, size_t qr, long x) {
        return _compute(0, x, ql, qr, 0, len2);
    }
    /*
    void update(size_t i, long x) {
        _update(0, i, x, 0, len2);
    } */
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
        //cin >> t;
        //if (t == 1) {
            cin >> l >> r >> x;
            x = tree.compute(l, r + 1, x);
            if (x == numeric_limits<long>::max()) {
                cout << "NONE" << '\n';
            } else {
                cout << x << '\n';
            }/*
        } else if (t == 2) {
            cin >> l >> x;
            tree.update(l, x);
        }*/
    }
    return 0;
}