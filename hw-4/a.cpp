#include <vector>
#include <future>
#include <execution>
#include <cmath>

struct Request {
    int left;
    int right;
};

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
            tree[v] = x;
            return;
        }
        size_t sm = (lb + rb) / 2;
        _update(left(v), i, x, lb, sm);
        _update(right(v), i, x, sm, rb);
        tree[v] = std::min(tree[left(v)], tree[right(v)]);
    }

    int _compute(size_t v, size_t ql, size_t qr, size_t lb, size_t rb) {
        if (rb <= ql or qr <= lb) {
            return std::numeric_limits<int>::max();
        }
        if (ql <= lb and qr >= rb) {
            return tree[v];
        }
        size_t sm = (rb + lb) / 2;
        return std::min(_compute(left(v), ql, qr, lb, sm),
               _compute(right(v), ql, qr, sm, rb));
    }

public:
    SegTree(const std::vector<int> &arr)
            : _arr(arr)
            , len2(pow2(arr.size()))
            , tree(pow2(arr.size()) * 2 - 1, std::numeric_limits<int>::max()) {
        for (size_t i = 0; i < arr.size(); ++i) {
            tree[i + len2 - 1] = arr[i];
        }
        for (int i = len2 - 2; i >= 0; --i) {
            tree[i] = std::min(tree[left(i)], tree[right(i)]);
        }
    }

    int compute(size_t ql, size_t qr) {
        return _compute(0, ql, qr, 0, len2);
    }

    void update(size_t i, long x) {
        _update(0, i, x, 0, len2);
    }
};

std::vector<int> ProcessRequests(const std::vector<int>& numbers,
                                 const std::vector<Request>& requests) {
    SegTree a(numbers);
    unsigned flows_count = std::thread::hardware_concurrency() != 0 ? std::thread::hardware_concurrency() : 10;
    size_t part_len = requests.size() / flows_count + 1;
    std::vector<int> ans;
    ans.resize(requests.size());
    std::vector<std::future<void>> flows(flows_count);
    for (int fi = 0; fi < flows_count; ++fi) {
        flows[fi] = std::async(std::launch::async, [&ans, &requests, &a, fi, part_len]{
            for (size_t i = fi * part_len; i < std::min((fi + 1) * part_len, requests.size()); ++i) {
                ans[i] = a.compute(requests[i].left, requests[i].right);
            }
        });
    }
    for(auto &i : flows) {
        i.get();
    }
    return ans;
}

/*std::vector<int> ProcessRequestsSimple(const std::vector<int>& numbers,
                                 const std::vector<Request>& requests) {
    SegTree a(numbers);
    std::vector<int> ans;
    ans.reserve(requests.size());
    for(auto& request : requests) {
        ans.push_back(a.compute(request.left, request.right));
    }
    return ans;
}*/