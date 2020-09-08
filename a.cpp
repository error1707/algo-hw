#include <iostream>
#include <vector>

class PrefixSum {
private:
    const std::vector<unsigned long long> &_arr;
    std::vector<unsigned long long> sum_arr;

public:
    PrefixSum(const std::vector<unsigned long long> &arr)
            : _arr(arr)
            , sum_arr(arr.size() + 1){
        for (size_t i = 0; i < _arr.size(); ++i) {
            sum_arr[i + 1] = sum_arr[i] + _arr[i];
        }
    }

    unsigned long long query(unsigned long long lb, unsigned long long rb) {
        return sum_arr[rb + 1] - sum_arr[lb];
    }
};

int main() {
    size_t n, q;
    unsigned long long x0, x1, l, r, s = 0;
    std::cin >> n;
    std::vector<unsigned long long> arr(n);

    for (auto &i : arr) {
        std::cin >> i;
    }

    PrefixSum a(arr);

    std::cin >> q;
    for (size_t i = 0; i < q; ++i) {
        if (i == 0) {
            std::cin >> x0;
        } else {
            x0 = (11173 * x1 + 1) % 1000000007;
        }
        x1 = (11173 * x0 + 1) % 1000000007;
        l = std::min(x0 % n, x1 % n);
        r = std::max(x0 % n, x1 % n);
        s += a.query(l, r) % 1000000007;
    }
    std::cout << s % 1000000007 << '\n';
    return 0;
}