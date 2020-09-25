#include <iostream>
#include <vector>
#include <map>
#include <set>

using namespace std;

vector<size_t> pref_func(const string &s) {
    vector<size_t> pref(s.length(), 0);
    int k;
    for (size_t i = 1; i < s.length(); ++i) {
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

class AutoKMP{
private:
    vector<map<char, size_t>> routes;
    const set<char> alph;
    const vector<size_t> pi;

public:
    AutoKMP(const string& s, const string& _alph)
        : alph(_alph.begin(), _alph.end())
        , pi(pref_func(s))
        , routes(s.length())
    {
        for (size_t i = 0; i < s.length(); ++i) {
            for(char ch : alph) {
                if(i > 0 && ch != s[i]) {
                    routes[i][ch] = routes[pi[i - 1]][ch];
                } else {
                    routes[i][ch] = ch == s[i] ? i + 1 : i;
                }
            }
        }
    }

    size_t max_cycle() {
        size_t ans = 0;
        for (size_t i = 1; i < routes.size(); ++i) {
            for(char ch : alph) {
                if(routes[i][ch] <= i) {
                    ans = max(ans, i - routes[i][ch]);
                }
            }
        }
        return ans;
    }
};

int main() {
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    size_t n, a, k;
    string word;
    cin >> n >> a >> k >> word;
    AutoKMP kmp(word, alphabet.substr(0, a));
    cout << n + k * kmp.max_cycle() << '\n';
    return 0;
}