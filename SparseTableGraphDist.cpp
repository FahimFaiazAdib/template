#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int MAXN = 100005; // Maximum number of nodes (1e5)
const int LOGN = 17;     // Maximum power of 2 needed (2^17 > 1e5)

// up[i][j] will store the node reached from node 'i' after 2^j hops.
int up[MAXN][LOGN];

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    cin >> n >> q;

    // Step 1: Read the graph and initialize the base case for binary lifting.
    // up[i][0] is the node you reach from 'i' in 2^0 = 1 hop.
    for (int i = 1; i <= n; ++i) {
        cin >> up[i][0];
    }

    // Step 2: Precomputation - Build the sparse table.
    // This uses the recurrence relation: up[i][j] = up[up[i][j-1]][j-1]
    for (int j = 1; j < LOGN; ++j) {
        for (int i = 1; i <= n; ++i) {
            up[i][j] = up[up[i][j-1]][j-1];
        }
    }

    // Step 3: Answer the queries.
    for (int i = 0; i < q; ++i) {
        int u, d;
        cin >> u >> d;

        // To find the destination after 'd' hops, we use the binary representation of 'd'.
        // For each set bit 'j' in 'd', we make a jump of 2^j.
        for (int j = 0; j < LOGN; ++j) {
            if ((d >> j) & 1) { // Check if the j-th bit of d is 1
                u = up[u][j];
            }
        }
        cout << u << "\n";
    }

    return 0;
}