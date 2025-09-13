#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Maximum grid size + sentinel borders
const int MAXN = 2005;

// Node structure for the 2D linked list
struct Node {
    int value;
    Node *up, *down, *left, *right;
};

// Global 2D array of Node objects
Node grid[MAXN][MAXN];

// Helper function to find a node by traversing pointers from the origin.
// This is the key to interacting with the dynamic grid.
Node* getNodeAt(int r, int c) {
    Node* current = &grid[0][0];
    // Traverse down to the target row's sentinel
    for (int i = 0; i < r; ++i) {
        current = current->down;
    }
    // Traverse right to the target node
    for (int j = 0; j < c; ++j) {
        current = current->right;
    }
    return current;
}

int main() {
    // Fast I/O for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    cin >> N >> M;

    // Initialize grid values
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= M; ++j) {
            cin >> grid[i][j].value;
        }
    }

    // Link all nodes to form the initial grid structure
    for (int i = 0; i <= N + 1; ++i) {
        for (int j = 0; j <= M + 1; ++j) {
            grid[i][j].up = &grid[i - 1][j];
            grid[i][j].down = &grid[i + 1][j];
            grid[i][j].left = &grid[i][j - 1];
            grid[i][j].right = &grid[i][j + 1];
        }
    }

    int Q;
    cin >> Q;

    while (Q--) {
        int r1, c1, r2, c2, h, w;
        cin >> r1 >> c1 >> r2 >> c2 >> h >> w;

        // --- Setup Phase: Find corners using your optimized idea ---

        // 1. Find the two main top-left anchor nodes by traversing
        Node* r1_topLeft = getNodeAt(r1, c1);
        Node* r2_topLeft = getNodeAt(r2, c2);

        // 2. Navigate from the anchors to find the other corners
        Node* r1_topRight = r1_topLeft;
        for (int i = 0; i < w - 1; ++i) r1_topRight = r1_topRight->right;

        Node* r2_topRight = r2_topLeft;
        for (int i = 0; i < w - 1; ++i) r2_topRight = r2_topRight->right;
        
        Node* r1_bottomLeft = r1_topLeft;
        for (int i = 0; i < h - 1; ++i) r1_bottomLeft = r1_bottomLeft->down;
        
        Node* r2_bottomLeft = r2_topLeft;
        for (int i = 0; i < h - 1; ++i) r2_bottomLeft = r2_bottomLeft->down;

        // --- Swap Phase: Iterate along boundaries using crawlers ---

        // 3. Swap horizontal connections
        Node* l_crawl1 = r1_topLeft->left;
        Node* r_crawl1 = r1_topRight->right;
        Node* l_crawl2 = r2_topLeft->left;
        Node* r_crawl2 = r2_topRight->right;
        
        for (int i = 0; i < h; ++i) {
            swap(l_crawl1->right, l_crawl2->right);
            swap(l_crawl1->right->left, l_crawl2->right->left);
            
            swap(r_crawl1->left, r_crawl2->left);
            swap(r_crawl1->left->right, r_crawl2->left->right);
            
            l_crawl1 = l_crawl1->down;
            r_crawl1 = r_crawl1->down;
            l_crawl2 = l_crawl2->down;
            r_crawl2 = r_crawl2->down;
        }

        // 4. Swap vertical connections
        Node* t_crawl1 = r1_topLeft->up;
        Node* b_crawl1 = r1_bottomLeft->down;
        Node* t_crawl2 = r2_topLeft->up;
        Node* b_crawl2 = r2_bottomLeft->down;

        for (int j = 0; j < w; ++j) {
            swap(t_crawl1->down, t_crawl2->down);
            swap(t_crawl1->down->up, t_crawl2->down->up);

            swap(b_crawl1->up, b_crawl2->up);
            swap(b_crawl1->up->down, b_crawl2->up->down);

            t_crawl1 = t_crawl1->right;
            b_crawl1 = b_crawl1->right;
            t_crawl2 = t_crawl2->right;
            b_crawl2 = b_crawl2->right;
        }
    }

    // Print the final grid by traversing from the top-left
    Node* current_row_head = grid[0][0].down;
    for (int i = 1; i <= N; ++i) {
        Node* curr = current_row_head->right;
        for (int j = 1; j <= M; ++j) {
            cout << curr->value << (j == M ? "" : " ");
            curr = curr->right;
        }
        cout << "\n";
        current_row_head = current_row_head->down;
    }

    return 0;
}