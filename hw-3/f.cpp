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

int ford_falk(int u, int min_c, vector<bool>& visited, map<int, map<int, Route>>& graph, int final) {
    if(u == final) {
        return min_c;
    }
    visited[u - 1] = true;
    for(auto& [v, edge] : graph.at(u)) {
        if(not visited[v - 1] and edge.f < edge.c) {
            int flow = ford_falk(v, min(min_c, edge.c - edge.f), visited, graph, final);
            if(flow > 0) {
                edge.f += flow;
                graph[v][u].f -= flow;
                return flow;
            }
        }
    }
    return 0;
}

int main() {
    map<int, map<int, Route>> graph;
    int n, m, to, from, c, new_flow, max_flow = 0;
    cin >> n >> m;
    for (int i = 0; i < m; ++i) {
        cin >> from >> to >> c;
        auto iter_from = graph.find(from);
        if(iter_from == graph.end()) {
            graph[from][to] = Route(c);
            graph[to][from] = Route(0);
        } else {
            auto iter_to = iter_from->second.find(to);
            if(iter_to == iter_from->second.end()) {
                iter_from->second[to] = Route(c);
                graph[to][from] = Route(0);
            } else {
                iter_to->second = Route(c);
            }
        }
    }

    vector<bool> visited(n, false);
    new_flow = ford_falk(1, numeric_limits<int>::max(), visited, graph, n);
    while(new_flow != 0) {
        max_flow += new_flow;
        visited.assign(n, false);
        new_flow = ford_falk(1, numeric_limits<int>::max(), visited, graph, n);
    }

    cout << max_flow << '\n';
    return 0;
}
