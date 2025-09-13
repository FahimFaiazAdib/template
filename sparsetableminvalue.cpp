#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

// Define constants based on problem constraints
const int MAXN = 100005;
const int LOGN = 17;
const int INF = 1e9 + 7;

// Adjacency list to represent the tree
vector<int> adj[MAXN];
// Array to store the value of each node
int val[MAXN];

// Sparse tables and depth array for LCA and path queries
int depth[MAXN];
int parent[MAXN][LOGN];    // parent[i][j] stores the 2^j-th ancestor of node i
int min_path[MAXN][LOGN];  // min_path[i][j] stores the minimum value on the path from i up 2^j steps

/**
 * @brief Performs a Depth First Search to compute depths and immediate parents.
 * This initializes the base cases (j=0) for our sparse tables.
 *
 * @param u The current node.
 * @param p The parent of the current node.
 * @param d The depth of the current node.
 */
void dfs(int u, int p, int d) {
    depth[u] = d;
    parent[u][0] = p;
    min_path[u][0] = val[u]; // Base case: a path of length 2^0 includes only the node itself.

    for (int v : adj[u]) {
        if (v != p) {
            dfs(v, u, d + 1);
        }
    }
}

/**
 * @brief Precomputes the sparse tables for parent and min_path using dynamic programming.
 * This step takes O(N log N) time.
 *
 * @param n The total number of nodes.
 */
void build_sparse_table(int n) {
    // Initialize base cases (j=0) using DFS, assuming node 1 is the root.
    dfs(1, 1, 0);

    // Fill the rest of the sparse tables
    for (int j = 1; j < LOGN; ++j) {
        for (int i = 1; i <= n; ++i) {
            // The 2^j-th ancestor is the 2^(j-1)-th ancestor of the 2^(j-1)-th ancestor
            parent[i][j] = parent[parent[i][j - 1]][j - 1];
            // The minimum on a path of length 2^j is the minimum of two halves of length 2^(j-1)
            min_path[i][j] = min(min_path[i][j - 1], min_path[parent[i][j - 1]][j - 1]);
        }
    }
}

/**
 * @brief Correctly finds the minimum value on the path between u and v in O(log N).
 *
 * @param u First node.
 * @param v Second node.
 * @return The minimum value on the path between u and v.
 */
int query(int u, int v) {
    int res = INF;

    // 1. Ensure u is the deeper node to simplify logic
    if (depth[u] < depth[v]) {
        swap(u, v);
    }

    // 2. Lift u up to the same level as v, updating the minimum value along the way.
    for (int j = LOGN - 1; j >= 0; --j) {
        if (depth[u] - (1 << j) >= depth[v]) {
            res = min(res, min_path[u][j]);
            u = parent[u][j];
        }
    }

    // 3. If v was an ancestor of the original u, u is now equal to v.
    if (u == v) {
        res = min(res, val[u]); // Include the value at the LCA itself
        return res;
    }

    // 4. Lift u and v up together until they are children of the LCA.
    for (int j = LOGN - 1; j >= 0; --j) {
        if (parent[u][j] != parent[v][j]) {
            res = min(res, min_path[u][j]);
            res = min(res, min_path[v][j]);
            u = parent[u][j];
            v = parent[v][j];
        }
    }
    
    // 5. The result must include the values at the final u, final v, and their common parent (the LCA).
    res = min({res, val[u], val[v], val[parent[u][0]]});
    
    return res;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    cin >> n >> q;

    for (int i = 1; i <= n; ++i) {
        cin >> val[i];
    }

    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Precomputation step
    build_sparse_table(n);

    // Process all queries
    for (int i = 0; i < q; ++i) {
        int u, v;
        cin >> u >> v;
        cout << query(u, v) << "\n";
    }

    return 0;
}