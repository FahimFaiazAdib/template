#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <stack>
#include <set>

// Use a multiset for the adjacency list for efficient edge removal (logarithmic time).
// This is crucial for performance with large graphs.
std::vector<std::multiset<int>> adj;
std::vector<int> degree;

// A simple DFS to check if all vertices with edges are connected.
void dfs(int u, std::vector<bool>& visited) {
    visited[u] = true;
    for (int v : adj[u]) {
        if (!visited[v]) {
            dfs(v, visited);
        }
    }
}

int main() {
    // Fast I/O for performance
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, m;
    std::cin >> n >> m;

    // Use 1-based indexing for vertices, so size is n+1
    adj.resize(n + 1);
    degree.resize(n + 1, 0);

    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        adj[u].insert(v);
        adj[v].insert(u);
        degree[u]++;
        degree[v]++;
    }

    // ============== 1. Pre-computation Checks ==============

    // Check the number of vertices with odd degrees.
    std::vector<int> odd_degree_nodes;
    for (int i = 1; i <= n; ++i) {
        if (degree[i] % 2 != 0) {
            odd_degree_nodes.push_back(i);
        }
    }

    // An Eulerian path/tour exists only if there are 0 or 2 odd-degree vertices.
    if (odd_degree_nodes.size() != 0 && odd_degree_nodes.size() != 2) {
        std::cout << "IMPOSSIBLE\n";
        return 0;
    }

    // Check for connectivity. All vertices with one or more edges must be in
    // the same connected component.
    int first_node = -1;
    for (int i = 1; i <= n; ++i) {
        if (!adj[i].empty()) {
            first_node = i;
            break;
        }
    }

    if (first_node != -1) { // Proceed only if the graph is not empty
        std::vector<bool> visited(n + 1, false);
        dfs(first_node, visited);

        for (int i = 1; i <= n; ++i) {
            if (!adj[i].empty() && !visited[i]) {
                std::cout << "IMPOSSIBLE\n"; // Graph is not connected
                return 0;
            }
        }
    }

    // ============== 2. Hierholzer's Algorithm ==============

    // Determine the starting node for the traversal.
    int start_node = 1; // Default start for a tour
    if (!odd_degree_nodes.empty()) {
        start_node = odd_degree_nodes[0]; // Must start at an odd-degree node for a path
    } else if (first_node != -1) {
        start_node = first_node; // For a tour, start at any node with edges
    }

    std::stack<int> st;
    st.push(start_node);
    std::vector<int> path;

    while (!st.empty()) {
        int u = st.top();

        if (adj[u].empty()) {
            // If there are no more edges from this vertex, it's the end of a cycle.
            // Add it to our final path.
            path.push_back(u);
            st.pop();
        } else {
            // Follow an edge to the next vertex.
            int v = *adj[u].begin();

            // Remove the edge (u, v) and (v, u) from the graph.
            adj[u].erase(adj[u].find(v));
            adj[v].erase(adj[v].find(u));
            
            // Add the new vertex to the stack to explore from.
            st.push(v);
        }
    }

    // ============== 3. Final Check and Output ==============

    // If the path contains all edges, print it. Otherwise, it's impossible.
    // The number of vertices in the path is always one more than the number of edges.
    if (path.size() != m + 1) {
        std::cout << "IMPOSSIBLE\n";
    } else {
        // The path is constructed in reverse, so we must reverse it.
        std::reverse(path.begin(), path.end());
        for (int i = 0; i < path.size(); ++i) {
            std::cout << path[i] << (i == path.size() - 1 ? "" : " ");
        }
        std::cout << "\n";
    }

    return 0;
}