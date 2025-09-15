#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAX_NODES = 200005;

vector<int> adj[MAX_NODES];
vector<int> disc, low;
vector<bool> is_bridge;
vector<int> component;
int timer, component_count;

/**
 * @brief Step 1: DFS to find all bridges in the graph.
 * @param u The current vertex being visited.
 * @param p The parent of the current vertex in the DFS tree.
 */
void find_bridges(int u, int p = -1) {
    disc[u] = low[u] = timer++;

    for (int v : adj[u]) {
        if (v == p) continue;

        if (disc[v] != -1) {
            low[u] = min(low[u], disc[v]);
        } else {
            find_bridges(v, u);
            low[u] = min(low[u], low[v]);
            if (low[v] > disc[u]) {
                // Mark the edge u-v as a bridge
                // Note: Storing bridges might be complex with parallel edges.
                // A common way is to use a map or pair of pairs.
                // For this code, we'll just conceptually remove them in the next step.
                // To do this simply, we will need to store edges and their indices.
                // The provided implementation takes a simpler route by re-traversing.
            }
        }
    }
}


/**
 * @brief Step 2: DFS to find connected components on the graph without bridges.
 * @param u The current vertex to visit.
 */
void find_components(int u) {
    component[u] = component_count;
    for (int v : adj[u]) {
        // A bridge is an edge (u, v) where low[v] > disc[u] or low[u] > disc[v]
        bool is_edge_a_bridge = (low[v] > disc[u] || low[u] > disc[v]);
        
        // Traverse the edge only if it's NOT a bridge and the neighbor is not visited
        if (!is_edge_a_bridge && component[v] == -1) {
            find_components(v);
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

    // --- Step 1: Find all bridges ---
    disc.assign(V, -1);
    low.assign(V, -1);
    timer = 0;
    for (int i = 0; i < V; ++i) {
        if (disc[i] == -1) {
            find_bridges(i);
        }
    }
    
    // --- Step 2: Find components in the graph without bridges ---
    component.assign(V, -1);
    component_count = 0;
    for (int i = 0; i < V; ++i) {
        if (component[i] == -1) {
            find_components(i);
            component_count++;
        }
    }

    // --- Print the results ---
    cout << "\nTotal Bridge Components found: " << component_count << endl;
    vector<vector<int>> result(component_count);
    for(int i = 0; i < V; ++i) {
        result[component[i]].push_back(i);
    }

    for (int i = 0; i < component_count; ++i) {
        cout << "Component " << i + 1 << ": ";
        for (int node : result[i]) {
            cout << node << " ";
        }
        cout << "\n";
    }

    return 0;
}