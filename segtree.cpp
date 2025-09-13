#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// The structure for each node in the segment tree.
struct Node {
    long long sum;
    int max_val;
};

vector<int> a;      // Initial array
vector<Node> tree;  // Segment tree

// Function to merge the results from two child nodes into their parent.
void merge(int v) {
    tree[v].sum = tree[2 * v].sum + tree[2 * v + 1].sum;
    tree[v].max_val = max(tree[2 * v].max_val, tree[2 * v + 1].max_val);
}

// Builds the segment tree from the initial array recursively.
// v: current vertex index in the tree
// [tl, tr]: range of the array this vertex represents [tree_left, tree_right]
void build(int v, int tl, int tr) {
    if (tl == tr) {
        // A leaf node represents a single element from the array
        tree[v].sum=a[tl];
        tree[v].max_val=a[tl];
    } else {
        int tm = tl + (tr - tl) / 2; // Midpoint to split the range
        build(v * 2, tl, tm);           // Recurse on the left child
        build(v * 2 + 1, tm + 1, tr);   // Recurse on the right child
        merge(v); // Pull information from children after they are built
    }
}

// Type 1 Query: Range update A[i] = min(A[i], X)
// [l, r]: the query range for the update
void update(int v, int tl, int tr, int l, int r, int x) {
    // The crucial optimization: if the max value in this node's range
    // is already less than or equal to x, this update is useless. Stop here.
    if (tree[v].max_val <= x) {
        return;
    }
    // If the current node represents a single element, perform the update.
    if (tl == tr) {
        tree[v].sum = min((long long)tree[v].sum, (long long)x);
        tree[v].max_val = min(tree[v].max_val, x);
        return;
    }

    int tm = tl + (tr - tl) / 2;
    // Recurse on left child if its range [tl, tm] overlaps with the query range [l, r]
    if (l <= tm) {
        update(v * 2, tl, tm, l, r, x);
    }
    // Recurse on right child if its range [tm+1, tr] overlaps with the query range [l, r]
    if (r > tm) {
        update(v * 2 + 1, tm + 1, tr, l, r, x);
    }
    // After children are potentially updated, merge their new info back up.
    merge(v);
}

// Type 2 Query: Range Sum
long long query_sum(int v, int tl, int tr, int l, int r) {
    // If the query range is completely outside the node's range, return 0.
    if (l > r) {
        return 0;
    }
    // If the query range completely covers the node's range, return its stored sum.
    if (l == tl && r == tr) {
        return tree[v].sum;
    }
    int tm = tl + (tr - tl) / 2;
    // Recursively query the children and add their results.
    long long left_sum = query_sum(v * 2, tl, tm, l, min(r, tm));
    long long right_sum = query_sum(v * 2 + 1, tm + 1, tr, max(l, tm + 1), r);
    return left_sum + right_sum;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    cin >> n;

    a.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    tree.resize(4 * n);
    build(1, 0, n - 1); // Build the tree starting at vertex 1, covering the whole array

    cin >> q;
    while (q--) {
        int type, l, r;
        cin >> type >> l >> r;
        // Adjusting from 1-based problem indexing to 0-based array indexing
        l--; 
        r--;

        if (type == 1) {
            int x;
            cin >> x;
            update(1, 0, n - 1, l, r, x);
        } else {
            cout << query_sum(1, 0, n - 1, l, r) << "\n";
        }
    }

    return 0;
}