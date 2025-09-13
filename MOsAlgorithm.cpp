#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>

using namespace std;

// --- Global variables ---
int BLOCK_SIZE;

// A struct to hold query information: Left, Right, and original index.
struct Query {
    int l, r, id;
};

// Custom comparator to sort queries for Mo's algorithm.
// It includes an optimization where the 'r' pointer direction alternates
// for adjacent blocks, reducing total pointer movement.
bool compare(const Query& a, const Query& b) {
    int block_a = a.l / BLOCK_SIZE;
    int block_b = b.l / BLOCK_SIZE;
    if (block_a != block_b) {
        return block_a < block_b;
    }
    // If block index is even, sort 'r' ascending.
    // If block index is odd, sort 'r' descending.
    return (block_a % 2) ? (a.r < b.r) : (a.r > b.r);
}

// --- Frequency tracking data structures ---
// freq[x]: stores the frequency of compressed number 'x'
vector<int> freq;
// freq_of_freq[k]: stores how many numbers have a frequency of 'k'
vector<int> freq_of_freq;
// The maximum frequency in the current window
int max_freq;

// --- Add/Remove functions for the sliding window ---

// Adds an element at index 'idx' to the current window
void add(int val) {
    // Decrement the count for the old frequency
    if (freq[val] > 0) {
        freq_of_freq[freq[val]]--;
    }
    
    // Increment the frequency of the number
    freq[val]++;
    
    // Increment the count for the new frequency
    freq_of_freq[freq[val]]++;
    
    // Update max_freq if necessary
    if (freq[val] > max_freq) {
        max_freq = freq[val];
    }
}

// Removes an element at index 'idx' from the current window
void remove(int val) {
    // Decrement the count for the old frequency
    freq_of_freq[freq[val]]--;
    
    // If we just removed the last number that had the max frequency,
    // the new max frequency must be one less.
    if (freq_of_freq[freq[val]] == 0 && freq[val] == max_freq) {
        max_freq--;
    }
    
    // Decrement the frequency of the number
    freq[val]--;
    
    // Increment the count for the new frequency
    if (freq[val] > 0) {
        freq_of_freq[freq[val]]++;
    }
}


int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    cin >> n >> q;

    vector<int> a(n);
    map<int, int> compression_map;
    int compressed_value = 0;

    // --- Coordinate Compression ---
    // Read input and prepare for compression
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        compression_map[a[i]] = 0; // Just to get unique keys
    }

    // Create the mapping from original value to new compressed value
    for (auto const& [val, placeholder] : compression_map) {
        compression_map[val] = compressed_value++;
    }

    // Apply the compression to the array
    for (int i = 0; i < n; ++i) {
        a[i] = compression_map[a[i]];
    }

    // --- Query Processing ---
    vector<Query> queries(q);
    for (int i = 0; i < q; ++i) {
        cin >> queries[i].l >> queries[i].r;
        queries[i].l--; // Convert to 0-based index
        queries[i].r--; // Convert to 0-based index
        queries[i].id = i;
    }

    // --- Mo's Algorithm ---
    BLOCK_SIZE = sqrt(n);
    sort(queries.begin(), queries.end(), compare);

    vector<pair<int, int>> answers(q);
    freq.assign(compressed_value + 1, 0);
    freq_of_freq.assign(n + 2, 0); // Max possible frequency is N
    max_freq = 0;
    
    int current_l = 0;
    int current_r = -1;

    for (const auto& query : queries) {
        int l = query.l;
        int r = query.r;

        // Extend the window
        while (current_l > l) {
            current_l--;
            add(a[current_l]);
        }
        while (current_r < r) {
            current_r++;
            add(a[current_r]);
        }
        
        // Shrink the window
        while (current_l < l) {
            remove(a[current_l]);
            current_l++;
        }
        while (current_r > r) {
            remove(a[current_r]);
            current_r--;
        }
        
        // Store the answer for the current query
        answers[query.id] = {max_freq, freq_of_freq[max_freq]};
    }

    // --- Output ---
    for (int i = 0; i < q; ++i) {
        cout << answers[i].first << " " << answers[i].second << "\n";
    }

    return 0;
}