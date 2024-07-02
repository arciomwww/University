#include <iostream>
#include <limits.h>
#include <list>
#include <queue>
#include <algorithm>
#include <fstream>
using namespace std;

// Структура для представления ребра с весом
struct Edge {
    int destination;
    int capacity;

    Edge(int dest, int cap) : destination(dest), capacity(cap) {}
};

class Graph {
    int V;  // Количество вершин графа
    list<Edge>* adj;  // Список смежности

public:
    Graph(int V);  // Конструктор

    // Добавление ребра к графу
    void addEdge(int u, int v, int capacity);

    // Поиск максимального потока из истока в сток
    int fordFulkerson(int source, int sink);

    int getV() { return V; }
};

Graph::Graph(int V) {
    this->V = V;
    adj = new list<Edge>[V];
}

void Graph::addEdge(int u, int v, int capacity) {
    adj[u].push_back(Edge(v, capacity));
    adj[v].push_back(Edge(u, 0));
}

bool bfs(list<Edge>* residualGraph, int source, int sink, int parent[]) {
    vector<bool> visited(100, false);
    queue<int> q;

    visited[source] = true;
    q.push(source);
    parent[source] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (const auto& edge : residualGraph[u]) {
            int v = edge.destination;
            int cap = edge.capacity;

            if (!visited[v] && cap > 0) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }

    return visited[sink];
}

int Graph::fordFulkerson(int source, int sink) {
    int* parent = new int[V];
    int max_flow = 0;

    list<Edge>* residualGraph = new list<Edge>[V];

    // Копируем значения из исходного графа в резервный граф
    for (int u = 0; u < V; u++) {
        for (const auto& edge : adj[u]) {
            int v = edge.destination;
            int capacity = edge.capacity;
            residualGraph[u].push_back(Edge(v, capacity));
        }
    }

    while (bfs(residualGraph, source, sink, parent)) {
        int path_flow = INT_MAX;

        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            auto it = find_if(residualGraph[u].begin(), residualGraph[u].end(), [v](const Edge& edge) { return edge.destination == v; });
            path_flow = min(path_flow, it->capacity);
        }

        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            auto it_uv = find_if(residualGraph[u].begin(), residualGraph[u].end(), [v](const Edge& edge) { return edge.destination == v; });
            it_uv->capacity -= path_flow;

            auto it_vu = find_if(residualGraph[v].begin(), residualGraph[v].end(), [u](const Edge& edge) { return edge.destination == u; });
            it_vu->capacity += path_flow;
        }

        max_flow += path_flow;
    }

    delete[] parent;
    delete[] residualGraph;

    return max_flow;
}

int main() {
    ifstream in("input.txt");

    int V;
    in >> V;
    Graph graph(V);
    int u, v, w;
    while (in >> u >> v >> w) {
        graph.addEdge(u, v, w);
    }
    int source = 0, sink = V - 1;
    cout << "Max flow: " << graph.fordFulkerson(source, sink) << endl;

    return 0;
}
