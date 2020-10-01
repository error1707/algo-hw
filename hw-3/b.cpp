#include <iostream>
#include <vector>
#include <set>

using namespace std;

bool dfs(int v, vector<bool>& used, const vector<set<int>>& routes, vector<int>& matching){
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

void kuhn(const vector<set<int>>& routes, vector<int>& matching) {
    vector<bool> used(routes.size());
    for (int i = 0; i < routes.size(); ++i) {
        used.assign(used.size(), false);
        dfs(i, used, routes, matching);
    }
}

void dfs_1(int u, set<int> &left, set<int> &right, vector<bool>& visited, const vector<set<int>> &routes, const vector<int> &matching) {
    left.insert(u);
    visited[u] = true;
    for(auto v : routes[u]) {
        right.erase(v);
        if(not visited[matching[v]]) {
            dfs_1(matching[v], left, right, visited, routes, matching);
        }
    }
}

int main() {
    int n, m, k, temp;
    cin >> k;
    for (int num = 0; num < k; ++num) {
        cin >> m >> n;
        set<int> right;
        for (int i = 0; i < n; ++i) {
            right.insert(i);
        }
        vector<set<int>> routes(m, right);
        for (int i = 0; i < m; ++i) {
            cin >> temp;
            while (temp != 0) {
                routes[i].erase(temp - 1);
                cin >> temp;
            }
        }
        vector<int> matching(n, -1);
        kuhn(routes, matching);
        set<int> free_left;
        for (int i = 0; i < m; ++i) {
            free_left.insert(i);
        }
        for (int i = 0; i < n; ++i) {
            if (matching[i] != -1) {
                free_left.erase(matching[i]);
            }
        }
        set<int> left;
        for (auto i : free_left) {
            vector<bool> visited(m, false);
            dfs_1(i, left, right, visited, routes, matching);
        }
        cout << left.size() + right.size() << '\n';
        cout << left.size() << ' ' << right.size() << '\n';
        for (auto i : left) {
            cout << i + 1 << ' ';
        }
        cout << '\n';
        for (auto i : right) {
            cout << i + 1 << ' ';
        }
        cout << '\n';
    }
    return 0;
}