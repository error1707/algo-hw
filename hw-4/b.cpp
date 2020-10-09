#include <vector>
#include <future>
#include <execution>
#include <cmath>
#include <variant>
#include <queue>

struct ComputeRequest {
    int left; int right;
};

struct UpdateRequest {
    int index; int delta;
};

using Request = std::variant<ComputeRequest, UpdateRequest>;

size_t pow2(size_t num) {
    size_t k = 1;
    while (k < num) {
        k <<= 1;
    }
    return k;
}

class SegTree {
private:
    const std::vector<int> &_arr;
    std::vector<int> tree;
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
            tree[v] += x;
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
    SegTree(const std::vector<int> &arr)
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

class SegTreeAtom {
private:
    const std::vector<int> &_arr;
    std::vector<std::atomic_int> tree;
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
            tree[v] += x;
            return;
        }
        size_t sm = (lb + rb) / 2;
        _update(left(v), i, x, lb, sm);
        _update(right(v), i, x, sm, rb);
        tree[v] += x;
    }

    int _compute(size_t v, size_t ql, size_t qr, size_t lb, size_t rb) {
        if (rb <= ql or qr <= lb) {
            return 0;
        }
        if (ql <= lb and qr >= rb) {
            return tree[v];
        }
        size_t sm = (rb + lb) / 2;
        return _compute(left(v), ql, qr, lb, sm) + _compute(right(v), ql, qr, sm, rb);
    }

public:
    SegTreeAtom(const std::vector<int> &arr)
            : _arr(arr)
            , len2(pow2(arr.size()))
            , tree(pow2(arr.size()) * 2 - 1) {
        for (size_t i = 0; i < len2; ++i) {
            if(i < arr.size()) {
                tree[i + len2 - 1] = arr[i];
            } else {
                tree[i + len2 - 1] = 0;
            }
        }
        for (int i = len2 - 2; i >= 0; --i) {
            tree[i] = tree[left(i)] + tree[right(i)];
        }
    }

    int compute(size_t ql, size_t qr) {
        return _compute(0, ql, qr, 0, len2);
    }

    void update(size_t i, long x) {
        _update(0, i, x, 0, len2);
    }
};

enum {
    Cmp,
    Upd
};

int rt(const Request& a) {
    return std::get_if<ComputeRequest>(&a) != nullptr ? Cmp : Upd;
}

std::vector<int> ProcessRequests(const std::vector<int>& numbers,
                            const std::vector<Request>& requests) {
    SegTreeAtom tree(numbers);
    std::vector<std::queue<int>> q(2);
    int prev = -1, cur, cmp_count = 0;
    for(const auto& i : requests) {
        if(prev == -1) {
            prev = rt(i);
            q[prev].push(1);
            if(prev == Cmp) {
                ++cmp_count;
            }
            continue;
        }
        cur = rt(i);
        if (prev == cur) {
            ++q[cur].back();
        } else {
            prev = cur;
            q[cur].push(1);
        }
        if(cur == Cmp) {
            ++cmp_count;
        }
    }
    std::vector<int> ans(cmp_count);
    cur = rt(requests[0]);
    prev = (cur + 1) % 2;
    size_t i = 0, ai = 0;
    int amount;
    while(not (q[Cmp].empty() and q[Upd].empty())) {
        switch (cur) {
            case Cmp:
                amount = q[Cmp].front();
                q[Cmp].pop();
                std::transform(std::execution::par,
                               requests.begin() + i,
                               requests.begin() + i + amount,
                               ans.begin() + ai,
                               [&tree](auto& req){
                    auto& [left, right] = std::get<ComputeRequest>(req);
                    return tree.compute(left, right);
                });
                i += amount;
                ai += amount;
                std::swap(cur, prev);
                break;
            case Upd:
                amount = q[Upd].front();
                q[Upd].pop();
                std::for_each_n(std::execution::par, requests.begin() + i, amount, [&tree](auto& req){
                    auto& [idx, value] = std::get<UpdateRequest>(req);
                    tree.update(idx, value);
                });
                i += amount;
                std::swap(cur, prev);
                break;
        }
    }
    return ans;
}

std::vector<int> ProcessRequestsSimple(const std::vector<int>& numbers,
                                 const std::vector<Request>& requests) {
    std::vector<int> ans;
    SegTree tree(numbers);
    ans.reserve(requests.size());
    for(auto &i : requests) {
        if(rt(i) == Cmp){
            auto tmp = std::get<ComputeRequest>(i);
            ans.push_back(tree.compute(tmp.left, tmp.right));
        } else if(rt(i) == Upd) {
            auto tmp = std::get<UpdateRequest>(i);
            tree.update(tmp.index, tmp.delta);
        }
    }
    return ans;
}