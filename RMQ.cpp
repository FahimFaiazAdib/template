#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

// Define constants based on problem constraints
// N <= 1e5, so MAXN = 100005
// LOGN = floor(log2(100005)) which is 16. We'll use 17 for safety.
const int MAXN = 100005;
const int LOGN = 17;

// The input array
int arr[MAXN];
// The sparse table
int st[MAXN][LOGN];
// Precomputed log values for O(1) query time
int log_table[MAXN];

/**
 * @brief Precomputes the sparse table for O(1) range minimum queries.
 *
 * @param n The size of the input array.
 */
void build_sparse_table(int n) {
    // 1. Initialize the base case (j=0): ranges of length 2^0 = 1
    for (int i = 0; i < n; ++i) {
        st[i][0] = arr[i];
    }

    // 2. Fill the rest of the table using dynamic programming
    // j represents the power of 2 for the range length (2^j)
    for (int j = 1; j < LOGN; ++j) {
        // i is the starting index of the range
        // We can only go up to n - (1 << j) to avoid going out of bounds
        for (int i = 0; i + (1 << j) <= n; ++i) {
            // The minimum of a range of size 2^j is the minimum of its two halves,
            // each of size 2^(j-1).
            st[i][j] = min(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]);
        }
    }
}

/**
 * @brief Precomputes log base 2 values for all numbers up to n.
 * This allows finding the correct 'j' for a given range length in O(1).
 *
 * @param n The size of the input array.
 */
void precompute_logs(int n) {
    log_table[1] = 0;
    for (int i = 2; i <= n; i++) {
        log_table[i] = log_table[i / 2] + 1;
    }
}

/**
 * @brief Answers a range minimum query in O(1).
 *
 * @param L The left index of the range (0-indexed).
 * @param R The right index of the range (0-indexed).
 * @return The minimum value in the subarray A[L...R].
 */
int query(int L, int R) {
    // Calculate the length of the range
    int len = R - L + 1;
    // Find the largest power of 2 that fits in this length
    int j = log_table[len];
    
    // The minimum is found by comparing two overlapping segments
    // that perfectly cover the range [L, R].
    return min(st[L][j], st[R - (1 << j) + 1][j]);
}


int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    cin >> n >> q;

    for (int i = 0; i < n; ++i) {
        cin >> arr[i];
    }
    
    // Precomputation steps
    precompute_logs(n);
    build_sparse_table(n);

    // Process all queries
    for (int i = 0; i < q; ++i) {
        int L, R;
        cin >> L >> R;
        // Assuming problem uses 1-based indexing for input, convert to 0-based
        // If it's 0-based, just use cin >> L >> R;
        cout << query(L - 1, R - 1) << "\n";
    }

    return 0;
}