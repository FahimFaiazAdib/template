#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// A structure to represent the Disjoint Set Union (DSU) or Union-Find data structure.
struct DSU {
    // parent[i] stores the parent of element i.
    std::vector<int> parent;
    // size[i] stores the size of the set that element i belongs to.
    std::vector<int> sz;
    // The number of disjoint sets or connected components.
    int components;

    // Constructor to initialize the DSU structure for 'n' elements.
    // Each element is initially in its own set.
    DSU(int n) {
        parent.resize(n + 1);
        // std::iota fills the range with sequentially increasing values.
        std::iota(parent.begin(), parent.end(), 0);
        sz.assign(n + 1, 1);
        components = n;
    }

    // Finds the representative (or root) of the set containing element 'i' with path compression.
    int find(int i) {
        if (parent[i] == i) {
            return i;
        }
        // Path compression: set the parent of i directly to the root.
        return parent[i] = find(parent[i]);
    }

    // Merges the sets containing elements 'i' and 'j' with union by size.
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        // If they are not already in the same set, merge them.
        if (root_i != root_j) {
            // Union by size: attach the smaller tree to the root of the larger tree.
            if (sz[root_i] < sz[root_j]) {
                std::swap(root_i, root_j);
            }
            parent[root_j] = root_i;
            sz[root_i] += sz[root_j];
            // Merging two components reduces the total number of components by one.
            components--;
        }
    }
};

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, m;
    std::cin >> n >> m; // N nodes, M edges

    // Store all edges and the queries (edges to be deleted)
    std::vector<std::pair<int, int>> edges(m);
    for (int i = 0; i < m; ++i) {
        std::cin >> edges[i].first >> edges[i].second;
    }

    std::vector<int> queries(m);
    // Keep track of which edges are removed in queries.
    std::vector<bool> is_removed(m + 1, false);
    for (int i = 0; i < m; ++i) {
        std::cin >> queries[i];
        // Adjust to 0-based index if your problem is 1-based.
        is_removed[queries[i]] = true;
    }

    // Initialize DSU. Initially, we have 'n' components.
    DSU dsu(n);

    // Build the initial graph: unite nodes for all edges that are NOT deleted in the queries.
    for (int i = 0; i < m; ++i) {
        // The problem statement edge indices are 1-based.
        if (!is_removed[i + 1]) {
            dsu.unite(edges[i].first, edges[i].second);
        }
    }

    // Process queries in reverse order (i.e., add edges back)
    std::vector<int> results;
    for (int i = m - 1; i >= 0; --i) {
        // First, record the number of components BEFORE adding the edge back.
        results.push_back(dsu.components);

        // Get the edge to add back (adjusting for 0-based vs 1-based indexing).
        int edge_idx = queries[i] - 1;
        dsu.unite(edges[edge_idx].first, edges[edge_idx].second);
    }

    // The results were stored in reverse order, so print them in the correct order.
    std::reverse(results.begin(), results.end());

    for (int i = 0; i < results.size(); ++i) {
        std::cout << results[i] << (i == results.size() - 1 ? "" : " ");
    }
    std::cout << std::endl;

    return 0;
}