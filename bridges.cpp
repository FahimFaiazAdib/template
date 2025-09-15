#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAX_NODES = 200005;

vector<int> adj[MAX_NODES];
vector<int> disc, low;
vector<pair<int, int>> bridges;
int timer;

/**
 * @brief A Depth First Search function to find bridges in the graph.
 * @param u The current vertex being visited.
 * @param p The parent of the current vertex in the DFS tree.
 */
void find_bridges(int u, int p = -1) {
    disc[u] = low[u] = timer++;

    for (int v : adj[u]) {
        // Ignore the edge leading back to the parent
        if (v == p) continue;

        if (disc[v] != -1) { // If v is already visited, it's a back edge
            low[u] = min(low[u], disc[v]);
        } else { // If v is not visited, it's a tree edge
            find_bridges(v, u);
            low[u] = min(low[u], low[v]);

            // This is the condition for an edge (u, v) to be a bridge
            if (low[v] > disc[u]) {
                bridges.push_back({u, v});
            }
        }
    }
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int V, E;
    cout << "Enter number of vertices and edges: ";
    cin >> V >> E;

    cout << "Enter " << E << " edges (u v):\n";
    for (int i = 0; i < E; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Initialize data structures
    disc.assign(V, -1);
    low.assign(V, -1);
    timer = 0;

    // Iterate through all vertices to handle disconnected graphs
    for (int i = 0; i < V; ++i) {
        if (disc[i] == -1) {
            find_bridges(i);
        }
    }

    cout << "\nBridges are:\n";
    if (bridges.empty()) {
        cout << "None\n";
    } else {
        for (const auto& edge : bridges) {
            cout << edge.first << " - " << edge.second << "\n";
        }
    }

    return 0;
}