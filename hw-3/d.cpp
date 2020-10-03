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

struct Client {
    int time, a, b, c, d;
};

int drive(Client c) {
    return abs(c.a - c.c) + abs(c.b - c.d);
}

int drive(Client f, Client s) {
    return abs(f.c - s.a) + abs(f.d - s.b);
}

int main() {
    int n, pairs = 0, from, to, h, m, start_time;
    char tmp;
    string s;
    cin >> n;
    vector<vector<int>> routes(n);
    vector<Client> clients(n);
    for (int i = 0; i < n; ++i) {
        cin >> h >> tmp >> m;
        clients[i].time = h * 60 + m;
        cin >> clients[i].a >> clients[i].b >> clients[i].c >> clients[i].d;
    }
    for (int i = 0; i < n - 1; ++i) {
        start_time = clients[i].time + drive(clients[i]);
        for (int j = i + 1; j < n; ++j) {
            if(start_time + drive(clients[i], clients[j]) < clients[j].time) {
                routes[i].push_back(j);
            }
        }
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