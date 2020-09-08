#include <iostream>
#include <vector>

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
    vector<long> tree;
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

    void _update(size_t v, size_t i, long x, size_t lb, size_t rb) {
        if (!(lb <= i and i < rb)) {
            return;
        }
        if (v == i + len2 - 1) {
            tree[v] = x;
            return;
        }
        size_t sm = (lb + rb) / 2;
        _update(left(v), i, x, lb, sm);
        _update(right(v), i, x, sm, rb);
        tree[v] = tree[left(v)] + tree[right(v)];
    }

    long _compute(size_t v, size_t ql, size_t qr, size_t lb, size_t rb) {
        if (rb <= ql or qr <= lb) {
            return 0;
        }
        if (ql <= lb and qr >= rb) {
            return tree[v];
        }
        size_t sm = (rb + lb) / 2;
        return _compute(left(v), ql, qr, lb, sm) +
               _compute(right(v), ql, qr, sm, rb);
    }

public:
    SegTree(const vector<long> &arr)
        : _arr(arr)
        , len2(pow2(arr.size()))
        , tree(pow2(arr.size()) * 2 - 1, 0) {
        for (size_t i = 0; i < arr.size(); ++i) {
            tree[i + len2 - 1] = arr[i];
        }
        for (int i = len2 - 2; i >= 0; --i) {
            tree[i] = tree[left(i)] + tree[right(i)];
        }
    }

    long compute(size_t ql, size_t qr) {
        return _compute(0, ql, qr, 0, len2);
    }

    void update(size_t i, long x) {
        _update(0, i, x, 0, len2);
    }
};

int main() {
    long q, t, x0, x1;
    size_t n;
    cin >> n;
    vector<long> arr(n);
    for (auto &i : arr) {
        cin >> i;
    }

    SegTree tree(arr);

    cin >> q;
    for (int i = 0; i < q; ++i) {
        cin >> t >> x0 >> x1;
        if (t == 1) {
            cout << tree.compute(x0, x1 + 1) << '\n';
        } else if (t == 2) {
            tree.update(x0, x1);
        }
    }
    return 0;
}