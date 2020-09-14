#include <iostream>
#include <vector>

using namespace std;

vector<int> pref_func(const string &s) {
    vector<int> pref(s.length(), 0);
    int k;
    for (int i = 1; i < s.length(); ++i) {
        k = pref[i - 1];
        while(k > 0 && s[i] != s[k]) {
            k = pref[k - 1];
        }
        if(s[i] == s[k]) {
            pref[i] = ++k;
        }
    }
    return pref;
}

int main() {
    string s;
    cin >> s;
    for (int i : pref_func(s)) {
        cout << i << ' ';
    }
    cout << "\n";
    return 0;
}