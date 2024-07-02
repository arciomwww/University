#include <iostream>
#include <vector>
#include <set>
#include <fstream>

using namespace std;

class Graph {
public:
    int V; // количество вершин
    vector<set<int>> adj; // список смежности

    Graph(int vertices) : V(vertices), adj(vertices) {}

    void addEdge(int v, int w) {
        adj[v].insert(w);
        adj[w].insert(v);
    }

    void greedyColoring() {
        vector<int> result(V, -1); // массив для хранения цветов вершин

        // Цвет первой вершины
        result[0] = 0;

        // Доступные цвета (индексы в массиве)
        vector<bool> available(V, false);

        // Раскрашиваем остальные вершины
        for (int u = 1; u < V; u++) {
            // Метим все доступные цвета
            for (auto neighbor : adj[u]) {
                if (result[neighbor] != -1) {
                    available[result[neighbor]] = true;
                }
            }

            // Находим первый доступный цвет
            int color;
            for (color = 0; color < V; color++) {
                if (!available[color]) {
                    break;
                }
            }

            result[u] = color; // Присваиваем вершине найденный цвет

            // Сбрасываем доступность цветов для следующей итерации
            for (auto neighbor : adj[u]) {
                if (result[neighbor] != -1) {
                    available[result[neighbor]] = false;
                }
            }
        }
        // Выводим раскраску
        cout << "minimal number of shafts: " << *max_element(result.begin(), result.end()) + 1 << endl;
    }
};

int main() {
    ifstream in("task4.txt");
    if (in.is_open()) {
        int NumV;
        in >> NumV;
        Graph g(NumV);
        int V, U;
        while (in >> V >> U) {
            g.addEdge(V - 1, U - 1);
        }
        g.greedyColoring();
    }
    return 0;
}
