#include <iostream>
#include <vector>
#include <set>

using namespace std;

vector<int> z_func(const string &s) {
    vector<int> z(s.length(), 0);
    size_t l = 0, r = 0;
    for (size_t i = 1; i != s.length(); ++i) {
        if (i <= r) {
            z[i] = min<int>(r - i + 1, z[i - l]);
        }
        while (i + z[i] < s.size() && s[z[i]] == s[i + z[i]]) {
            ++z[i];
        }
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

int main() {
    string p, t;
    cin >> p;
    cin >> t;
    if(p.length() > t.length()) {
        cout << 0 << '\n';
        return 0;
    }
    vector<int> pi = z_func(p + '#' + t);
    vector<int> pi_rev = z_func(string(p.rbegin(), p.rend()) + '#' + string(t.rbegin(), t.rend()));
    vector<size_t> ans;
    for(size_t i = 0; i < pi.size() - 2 * p.length(); ++i) {
        if(pi[i + p.length() + 1] + pi_rev[pi_rev.size() - p.length() - i] >= p.length() - 1) {
            ans.push_back(i + 1);
        }
    }
    cout << ans.size() << '\n';
    for(auto i : ans) {
        cout << i << ' ';
    }
    cout << '\n';
    return 0;
}