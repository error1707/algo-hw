#include <iostream>
#include <map>
#include <vector>
#include <limits>

using namespace std;

struct Route {
    int f = 0, c = 0;
    Route(int _c) : c(_c) {}
    Route() {}
};

int ford_falk(int u, int final, vector<bool> &visited, map<int, map<int, Route>> &graph) {
    if(u == final) {
        return 1;
    }
    visited[u - 1] = true;
    for(auto& [v, edge] : graph.at(u)) {
        if(not visited[v - 1] and edge.f < edge.c) {
            int flow = ford_falk(v, final, visited, graph);
            if(flow > 0) {
                edge.f += flow;
                graph[v][u].f -= flow;
                return flow;
            }
        }
    }
    return 0;
}

void decompose(int u, int final, map<int, map<int, Route>> &graph, vector<int> &way) {
    if(u == final) {
        return;
    }
    for(auto& [v, edge] : graph.at(u)) {
        if(edge.f > 0) {
            edge.f -= 1;
            graph[v][u].f += 1;
            way.push_back(v);
            decompose(v, final, graph, way);
            return;
        }
    }
}

int main() {
    map<int, map<int, Route>> graph;
    int n, m, start, finish, to, from, max_flow = 0;
    cin >> n >> m >> start >> finish;
    for (int i = 0; i < m; ++i) {
        cin >> from >> to;
        if (from == to) {
            continue;
        }
        auto iter_from = graph.find(from);
        if (iter_from == graph.end()) {
            graph[from][to] = Route(1);
            graph[to][from] = Route(0);
        } else {
            auto iter_to = iter_from->second.find(to);
            if (iter_to == iter_from->second.end()) {
                iter_from->second[to] = Route(1);
                graph[to][from] = Route(0);
            } else {
                iter_to->second.c += 1;
            }
        }
    }
    vector<bool> visited(n, false);
    max_flow += ford_falk(start, finish, visited, graph);
    visited.assign(n, false);
    max_flow += ford_falk(start, finish, visited, graph);
    if(max_flow < 2) {
        cout << "NO\n";
        return 0;
    }
    cout << "YES\n";
    vector<int> one_way = {start};
    decompose(start, finish, graph, one_way);
    vector<int> another_way = {start};
    decompose(start, finish, graph, another_way);
    if(one_way.size() < another_way.size()) {
        for(auto i : one_way) {
            cout << i << ' ';
        }
        cout << '\n';
        for(auto i : another_way) {
            cout << i << ' ';
        }
        cout << '\n';
    } else {
        for(auto i : another_way) {
            cout << i << ' ';
        }
        cout << '\n';
        for(auto i : one_way) {
            cout << i << ' ';
        }
        cout << '\n';
    }
    return 0;
}
