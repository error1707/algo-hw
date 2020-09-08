#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

unsigned pow2(unsigned num) {
    unsigned k = 1, i = 0;
    while (k <= num) {
        k <<= 1;
        ++i;
    }
    return i - 1;
}

class SparseTable {
private:
    const vector<unsigned long> &_arr;
    vector<vector<unsigned long>> table;

public:
    SparseTable(const vector<unsigned long> &arr)
        : _arr(arr)
        , table(pow2(arr.size()) + 1){
        table[0] = _arr;
        for (int k = 1; k < table.size(); ++k) {
            table[k].resize(_arr.size() - pow(2, k) + 1);
            for (int i = 0; i < table[k].size(); ++i) {
                table[k][i] = max(table[k-1][i], table[k-1][i + pow(2, k - 1)]);
            }
        }
    }

    unsigned long query(size_t l, size_t r) {
        unsigned k = pow2(r - l + 1);
        return max(table[k][l], table[k][r - pow(2, k) + 1]);
    }
};

int main() {
    size_t n, q;
    unsigned long x0, x1, l, r, s = 0;
    std::cin >> n;
    std::vector<unsigned long> arr(n);

    for (auto &i : arr) {
        std::cin >> i;
    }

    SparseTable a(arr);

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