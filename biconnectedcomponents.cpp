#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;

const int MAX_NODES = 200005;

vector<int> adj[MAX_NODES];
vector<int> disc, low;
stack<pair<int, int> > edge_stack;
vector<vector<pair<int, int> > > bccs; // To store the components
int timer;

/**
 * @brief DFS function to find biconnected components.
 * @param u The current vertex being visited.
 * @param p The parent of the current vertex in the DFS tree.
 */
void find_bccs(int u, int p = -1) {
    disc[u] = low[u] = timer++;

    for (int v : adj[u]) {
        // We consider the edge (u, v) only once.
        // If v is parent, it's already handled.
        if (v == p) continue;

        // If v is not visited yet, push edge (u, v) to stack and recurse.
        // We check disc[v] < disc[u] to ensure we only push an edge once for undirected graphs.
        if (disc[v] == -1) {
            edge_stack.push(make_pair(u,v));
            find_bccs(v, u);

            // After recursion, update low-link value of u.
            low[u] = min(low[u], low[v]);

            // If u is an articulation point (or the root), it separates a BCC.
            if (low[v] >= disc[u]) {
                vector<pair<int, int> > current_bcc;
                while (true) {
                    pair<int, int> current_edge = edge_stack.top();
                    edge_stack.pop();
                    current_bcc.push_back(current_edge);
                    if (current_edge.first == u && current_edge.second == v) {
                        break;
                    }
                }
                bccs.push_back(current_bcc);
            }
        } 
        // If v is already visited, it's a back edge. Update low[u].
        // Also push the back edge to the stack if it represents a new path upwards.
        else if (disc[v] < disc[u]) {
             low[u] = min(low[u], disc[v]);
             edge_stack.push(make_pair(u,v));
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
            find_bccs(i);
            
            // Any remaining edges on the stack form the last BCC
            if (!edge_stack.empty()) {
                vector<pair<int, int> > last_bcc;
                while (!edge_stack.empty()) {
                    last_bcc.push_back(edge_stack.top());
                    edge_stack.pop();
                }
                bccs.push_back(last_bcc);
            }
        }
    }

    cout << "\nBiconnected Components are:\n";
    for (int i = 0; i < bccs.size(); ++i) {
        cout << "Component " << i + 1 << ": ";
        for (const auto& edge : bccs[i]) {
            cout << "(" << edge.first << "-" << edge.second << ") ";
        }
        cout << "\n";
    }

    return 0;
}