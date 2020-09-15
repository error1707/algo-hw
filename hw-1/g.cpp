#include <iostream>
#include <vector>

using namespace std;

long is_even(long num) {
    num = num > 0 ? num : -num;
    return (num + 1) % 2;
}

size_t pow2(size_t num) {
    size_t k = 1;
    while (k < num) {
        k <<= 1;
    }
    return k;
}

class SegTree {
private:
    struct node {
        long count = 0, pref = 0, suf = 0, max_seg = 0;
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

    node join(const node& l, const node& r) {
        return {
            l.count + r.count,
            l.pref == l.count ? l.count + r.pref : l.pref,
            r.suf == r.count ? r.count + l.suf : r.suf,
            max(l.suf + r.pref, max(l.max_seg, r.max_seg))
        };
    }

    void _update(size_t v, size_t i, long x, size_t lb, size_t rb) {
        if (!(lb <= i and i < rb)) {
            return;
        }
        if (v == i + len2 - 1) {
            tree[v].pref = tree[v].suf = tree[v].max_seg = x;
            return;
        }
        size_t sm = (lb + rb) / 2;
        _update(left(v), i, x, lb, sm);
        _update(right(v), i, x, sm, rb);
        tree[v] = join(tree[left(v)], tree[right(v)]);
    }

    node _compute(size_t v, size_t ql, size_t qr, size_t lb, size_t rb) {
        if (rb <= ql or qr <= lb) {
            return {};
        }
        if (ql <= lb and qr >= rb) {
            return tree[v];
        }
        size_t sm = (rb + lb) / 2;
        return join(_compute(left(v), ql, qr, lb, sm),
               _compute(right(v), ql, qr, sm, rb));
    }

public:
    SegTree(const vector<long> &arr)
            : _arr(arr)
            , len2(pow2(arr.size()))
            , tree(pow2(arr.size()) * 2 - 1) {
        for (size_t i = 0; i < arr.size(); ++i) {
            tree[i + len2 - 1].count = 1;
            if(is_even(arr[i]) == 1) {
                tree[i + len2 - 1].pref = tree[i + len2 - 1].suf = tree[i + len2 - 1].max_seg = 1;
            }
        }
        for (int i = len2 - 2; i >= 0; --i) {
            tree[i] = join(tree[left(i)], tree[right(i)]);
        }
    }

    long compute(size_t ql, size_t qr) {
        return _compute(0, ql, qr, 0, len2).max_seg;

    }

    void update(size_t i, long x) {
        _update(0, i, is_even(x), 0, len2);
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