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
    struct node{
        long sum = 0, add = 0;
        long val() {
            return sum + add;
        }
    };

    const vector<long> &_arr;
    vector<node> tree;
    size_t len2;

    size_t left(size_t v) {
        return 2 * v + 1;
    }

    size_t right(size_t v) {
        return 2 * v + 2;
    }

    void _update(size_t v, long x, size_t ql, size_t qr, size_t lb, size_t rb) {
        if (rb <= ql or qr <= lb) {
            return;
        }
        if (ql <= lb and rb <= qr) {
            tree[v].add += x * (rb - lb);
            return;
        }
        push(v);
        size_t sm = (lb + rb) / 2;
        _update(left(v), x, ql, qr, lb, sm);
        _update(right(v), x, ql, qr, sm, rb);
        tree[v].sum = tree[left(v)].val() + tree[right(v)].val();
    }

    long _compute(size_t v, size_t ql, size_t qr, size_t lb, size_t rb) {
        if (rb <= ql or qr <= lb) {
            return 0;
        }
        if (ql <= lb and qr >= rb) {
            return tree[v].val();
        }
        push(v);
        size_t sm = (rb + lb) / 2;
        return _compute(left(v), ql, qr, lb, sm) +
               _compute(right(v), ql, qr, sm, rb);
    }

    void push(size_t v) {
        if (len2 - 1 <= v and v < tree.size()) {
            return;
        }
        tree[left(v)].add += tree[v].add / 2;
        tree[right(v)].add += tree[v].add / 2;
        tree[v].add = 0;
        tree[v].sum = tree[left(v)].val() + tree[right(v)].val();
    }

public:
    SegTree(const vector<long> &arr)
            : _arr(arr)
            , len2(pow2(arr.size()))
            , tree(pow2(arr.size()) * 2 - 1) {
        for (size_t i = 0; i < arr.size(); ++i) {
            tree[i + len2 - 1].sum = arr[i];
        }
        for (int i = len2 - 2; i >= 0; --i) {
            tree[i].sum = tree[left(i)].val() + tree[right(i)].val();
        }
    }

    long compute(size_t ql, size_t qr) {
        return _compute(0, ql, qr, 0, len2);
    }

    void update(long x, size_t ql, size_t qr) {
        _update(0, x, ql, qr, 0, len2);
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
        cin >> t >> l >> r;
        if (t == 1) {
            cout << tree.compute(l, r + 1) << '\n';
        } else if (t == 2) {
            cin >> x;
            tree.update(x, l, r + 1);
        }
    }
    return 0;
}