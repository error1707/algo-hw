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
    int n, m, pairs = 0, from, to;
    string s;
    cin >> n >> m;
    vector<vector<int>> routes(n);
    for (int i = 0; i < m; ++i) {
        cin >> from >> to;
        routes[from - 1].push_back(to - 1);
    }
    vector<int> matching(n, -1);
    kuhn(routes, matching);
    for(auto i : matching) {
        if(i != -1) {
            ++pairs;
        }
    }
    cout << n - pairs << '\n';
    return 0;
}