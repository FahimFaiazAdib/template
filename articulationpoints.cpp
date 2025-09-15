#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

const int MAX_NODES = 200005;

vector<int> adj[MAX_NODES];
vector<int> disc, low;
set<int> articulation_points;
int timer;

/**
 * @brief A Depth First Search function to find articulation points.
 * @param u The current vertex being visited.
 * @param p The parent of the current vertex in the DFS tree.
 */
void find_aps(int u, int p = -1) {
    disc[u] = low[u] = timer++;
    int children = 0; // Count of children in the DFS tree

    for (int v : adj[u]) {
        // Ignore the edge leading back to the parent
        if (v == p) continue;

        if (disc[v] != -1) { // If v is visited, it's a back edge
            low[u] = min(low[u], disc[v]);
        } else { // If v is not visited, it's a tree edge
            children++;
            find_aps(v, u);
            low[u] = min(low[u], low[v]);

            // --- Articulation Point Conditions ---
            // Case 1: u is the root of the DFS tree and has more than one child.
            if (p == -1 && children > 1) {
                articulation_points.insert(u);
            }
            // Case 2: u is not the root, and the low-link value of a child v
            // is greater than or equal to the discovery time of u.
            if (p != -1 && low[v] >= disc[u]) {
                articulation_points.insert(u);
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
            find_aps(i);
        }
    }

    cout << "\nArticulation Points are:\n";
    if (articulation_points.empty()) {
        cout << "None\n";
    } else {
        for (int point : articulation_points) {
            cout << point << " ";
        }
        cout << "\n";
    }

    return 0;
}