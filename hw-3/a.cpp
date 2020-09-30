#include <iostream>
#include <vector>

using namespace std;

bool dfs(int v, vector<bool>& used, const vector<vector<int>>& routes, vector<int>& matching){
    if(used[v]) {
        return false;
    }
    used[v] = true;
    for(auto to : routes[v]) {
        if(matching[to] == -1 or dfs(matching[to], used, routes, matching)) {
            matching[to] = v;
            return true;
        }
    }
    return false;
}

void kuhn(const vector<vector<int>>& routes, vector<int>& matching) {
    vector <bool> used(routes.size());
    for (int i = 0; i < routes.size(); ++i) {
        used.assign(used.size(), false);
        dfs(i, used, routes, matching);
    }
}

int main() {
    int n, m, pairs = 0, temp;
    string s;
    cin >> n >> m;
    vector<vector<int>> routes(n);
    for (int i = 0; i < n; ++i) {
        cin >> temp;
        while(temp != 0) {
            routes[i].push_back(temp - 1);
            cin >> temp;
        }
    }
    vector<int> matching(m, -1);
    kuhn(routes, matching);
    for(auto i : matching) {
        if(i != -1) {
            ++pairs;
        }
    }
    cout << pairs << '\n';
    for (int i = 0; i < m; ++i) {
        if(matching[i] != -1) {
            cout << matching[i] + 1 << ' '  << i + 1 << '\n';
        }
    }
    return 0;
}