#include <iostream>
#include <vector>
#include <numeric>

// Maximum possible value in the array or queries. Adjust if necessary.
const int MAX_VALUE = 200005; // N + Q is a safe upper bound

struct DSU {
    std::vector<int> parent;
    
    // Constructor: Initially, every value's representative is itself.
    DSU(int n) {
        parent.resize(n + 1);
        std::iota(parent.begin(), parent.end(), 0);
    }

    // Finds the representative of the set containing 'i' with path compression.
    int find(int i) {
        if (parent[i] == i) {
            return i;
        }
        return parent[i] = find(parent[i]);
    }

    // Merges the sets containing values 'i' and 'j'.
    // We make the representative of j's set the parent of the representative of i's set.
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            parent[root_i] = root_j;
        }
    }
};

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    // The initial array, which we will not modify.
    std::vector<int> initial_array(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> initial_array[i];
    }

    // Initialize the DSU for all possible values.
    DSU value_dsu(MAX_VALUE);

    int q;
    std::cin >> q;
    while (q--) {
        int type;
        std::cin >> type;
        if (type == 1) {
            int x, y;
            std::cin >> x >> y;
            // "replace all x into y" means we merge the set of x into the set of y.
            if (x != y) {
                value_dsu.unite(x, y);
            }
        } else { // type == 2
            int idx;
            std::cin >> idx;
            // Adjust for 0-based indexing if problem is 1-based.
            // int original_value = initial_array[idx-1]; 
            int original_value = initial_array[idx]; 

            // Find the final value after all transformations.
            int final_value = value_dsu.find(original_value);
            std::cout << final_value << "\n";
        }
    }

    return 0;
}