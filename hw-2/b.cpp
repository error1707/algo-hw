#include <iostream>
#include <vector>

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
    string s;
    cin >> s;
    for (int i : z_func(s)) {
        cout << i << ' ';
    }
    cout << "\n";
    return 0;
}