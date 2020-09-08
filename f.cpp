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
    vector<long> _arr;
    vector<long> tree;
    size_t len2;

    struct helper {
        long sum, idx;
        helper(long _sum, long _idx)
                : sum(_sum)
                , idx(_idx) {}
    };

    size_t left(size_t v) {
        return 2 * v + 1;
    }

    size_t right(size_t v) {
        return 2 * v + 2;
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

    helper _compute(size_t v, long x, size_t ql, size_t qr, size_t lb, size_t rb) {
        if (rb <= ql or qr <= lb) {
            return helper(0, -1);
        }
        if ((len2 - 1 <= v and v < tree.size()) or len2 == 1) {
            if (tree[v] == 0) {
                return helper(0, -1);
            }
            if (x == 1) {
                return helper(1, v);
            } else {
                return helper(1, -1);
            }
        }
        if (ql <= lb and qr >= rb and tree[v] < x) {
            return helper(tree[v], -1);
        }
        size_t sm = (rb + lb) / 2;
        helper left_br = _compute(left(v), x, ql, qr, lb, sm);
        if (left_br.idx != -1) {
            return left_br;
        }
        helper right_br = _compute(right(v), x - left_br.sum, ql, qr, sm, rb);
        return helper(left_br.sum + right_br.sum, left_br.idx == -1 ? right_br.idx : left_br.idx);
    }

public:
    SegTree(const vector<long> &arr)
            : _arr(arr)
            , len2(pow2(arr.size()))
            , tree(pow2(arr.size()) * 2 - 1, 0) {
        for (size_t i = 0; i < arr.size(); ++i) {
            tree[i + len2 - 1] = is_even(arr[i]);
        }
        for (int i = len2 - 2; i >= 0; --i) {
            tree[i] = tree[left(i)] + tree[right(i)];
        }
    }

    long compute(size_t ql, size_t qr, long x) {
        if (x > tree[0] or x < 1) {
            return -1;
        }
        return _compute(0, x, ql, qr, 0, len2).idx;
    }

    void update(size_t i, long x) {
        _update(0, i, is_even(x), 0, len2);
        _arr[i] = x;
    }

    long operator[](size_t i) {
        return _arr[i - len2 + 1];
    }
};

int main() {
    long q, t, idx, elem, l, r;
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
            cin >> l >> r >> idx;
            idx = tree.compute(l, r + 1, idx);
            if (idx == -1) {
                cout << "NONE" << '\n';
            } else {
                cout << tree[idx] << '\n';
            }
        } else if (t == 2) {
            cin >> idx >> elem;
            tree.update(idx, elem);
        }
    }
    return 0;
}