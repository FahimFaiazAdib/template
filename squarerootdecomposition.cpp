#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

const int INF = 2e9; // A large value for infinity
long long n;
int block_size;
vector<long long> a;
vector<long long> block_sum;
vector<int> block_max;

// Helper to re-calculate the aggregate data for a block
void rebuild_block(int b_idx) {
    block_sum[b_idx] = 0;
    block_max[b_idx] = -INF;
    int start = b_idx * block_size;
    int end = min((int)n, (b_idx + 1) * block_size);
    for (int i = start; i < end; ++i) {
        block_sum[b_idx] += a[i];
        block_max[b_idx] = max(block_max[b_idx], (int)a[i]);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n;
    a.resize(n);
    block_size = sqrt(n);
    int num_blocks = (n + block_size - 1) / block_size;
    block_sum.assign(num_blocks, 0);
    block_max.assign(num_blocks, -INF);

    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    // Initial build
    for (int i = 0; i < num_blocks; ++i) {
        rebuild_block(i);
    }

    int q;
    cin >> q;
    while (q--) {
        int type, l, r;
        cin >> type >> l >> r;
        l--; 
        r--;

        if (type == 1) {
            int x;
            cin >> x;
            int start_block = l / block_size;
            int end_block = r / block_size;

            if (start_block == end_block) {
                // The entire range is within one block
                for (int i = l; i <= r; ++i) {
                    a[i] = min(a[i], (long long)x);
                }
                rebuild_block(start_block);
            } else {
                // 1. Partial left block
                for (int i = l; i < (start_block + 1) * block_size; ++i) {
                    a[i] = min(a[i], (long long)x);
                }
                rebuild_block(start_block);

                // 2. Full blocks in the middle
                for (int b = start_block + 1; b < end_block; ++b) {
                    if (block_max[b] > x) { // The crucial optimization
                        for (int i = b * block_size; i < (b + 1) * block_size; ++i) {
                           a[i] = min(a[i], (long long)x);
                        }
                        rebuild_block(b);
                    }
                }

                // 3. Partial right block
                for (int i = end_block * block_size; i <= r; ++i) {
                    a[i] = min(a[i], (long long)x);
                }
                rebuild_block(end_block);
            }
        } else {
            long long total_sum = 0;
            int start_block = l / block_size;
            int end_block = r / block_size;

            if (start_block == end_block) {
                for (int i = l; i <= r; ++i) {
                    total_sum += a[i];
                }
            } else {
                // 1. Partial left block
                for (int i = l; i < (start_block + 1) * block_size; ++i) {
                    total_sum += a[i];
                }
                // 2. Full blocks
                for (int b = start_block + 1; b < end_block; ++b) {
                    total_sum += block_sum[b];
                }
                // 3. Partial right block
                for (int i = end_block * block_size; i <= r; ++i) {
                    total_sum += a[i];
                }
            }
            cout << total_sum << "\n";
        }
    }

    return 0;
}