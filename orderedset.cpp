#include <iostream>

// Required headers for the ordered set
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

// Use the namespace for PBDS
using namespace __gnu_pbds;

// Define the ordered_set type
// int: the type of data stored
// null_type: mapped policy (for set-like behavior)
// less<int>: comparator for sorting
// rb_tree_tag: the type of tree used (Red-Black Tree)
// tree_order_statistics_node_update: policy for order statistics
typedef tree<int, null_type, std::less<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;
// typedef tree<pair<int,int>, null_type, std::less<pair<int,int >>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;


/// you can put a ordered set inside a map if you want as value.
// find_by_order(0) basically o_set[0]
// order_of_key(x)=y means o_set[y-1]<x; so total y ta value strictly less than y. order_of_key returns integer.
// lower_bound(x) returns iterator. so comparing that you can see if exits or not
// you can use normal methods like .begin() , .end() , erase() etc

// for multiple values , just take <pair<int,int> > 

int main() {
    // Create an ordered_set instance
    ordered_set o_set;

    // Insert elements
    o_set.insert(10);
    o_set.insert(20);
    o_set.insert(30);
    o_set.insert(40);
    o_set.insert(50);

    // The set now contains {10, 20, 30, 40, 50}
    std::cout << "The set is: {10, 20, 30, 40, 50}" << std::endl;

    // ---- Main Features ----

    // 1. Find the k-th smallest element using find_by_order(k)
    //    (0-indexed)
    std::cout << "The 0-th element is: " << *o_set.find_by_order(0) << std::endl; // Output: 10
    std::cout << "The 2nd element is: " << *o_set.find_by_order(2) << std::endl; // Output: 30
    std::cout << "The 4th element is: " << *o_set.find_by_order(4) << std::endl; // Output: 50
    
    // 2. Find the number of items strictly less than a value using order_of_key(x)
    std::cout << "Number of elements less than 35: " << o_set.order_of_key(35) << std::endl; // Output: 3 (10, 20, 30)
    std::cout << "Number of elements less than 10: " << o_set.order_of_key(10) << std::endl; // Output: 0
    std::cout << "Number of elements less than 51: " << o_set.order_of_key(51) << std::endl; // Output: 5

    // You can also erase elements
    o_set.erase(20);
    std::cout << "After erasing 20, the 2nd element is now: " << *o_set.find_by_order(2) << std::endl; // Output: 40
    
    return 0;
}