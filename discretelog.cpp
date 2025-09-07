#include <iostream>
#include <cmath>
#include <map>
#include <numeric> // For std::gcd in C++17

/**
 * @brief Calculates (base^exp) % mod using binary exponentiation.
 */
long long power(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (__int128)res * base % mod;
        base = (__int128)base * base % mod;
        exp /= 2;
    }
    return res;
}

/**
 * @brief Solves for x in a^x ≡ b (mod m) using the Baby-step Giant-step algorithm.
 * * Note: This implementation assumes that a and m are coprime.
 * Time Complexity: O(sqrt(m) * log(m))
 * Space Complexity: O(sqrt(m))
 * * @param a The base.
 * @param b The result.
 * @param m The modulus.
 * @return The smallest non-negative integer x, or -1 if no solution exists.
 */
long long discreteLog(long long a, long long b, long long m) {
    // Ensure b is within [0, m-1]
    b %= m;
    a %= m;

    // Edge case: if a=0, solution exists only if b=0 (x>0) or b=1 (x=0)
    if (a == 0) {
        if (b == 0) return 1; // 0^1 = 0
        if (b == 1) return 0; // 0^0 = 1
        return -1;
    }

    // k = ceil(sqrt(m))
    long long k = sqrt(m) + 1;

    // Store hash of b * a^j
    std::map<long long, long long> baby_steps;

    // Baby steps: Precompute and store values of b * a^j
    long long current_val = b;
    for (long long j = 0; j < k; ++j) {
        // Use map.try_emplace to avoid overwriting with a larger j
        // We want the smallest j for a given value.
        baby_steps.try_emplace(current_val, j);
        current_val = (__int128)current_val * a % m;
    }

    // Giant steps: Calculate a^(i*k) and check for a match
    long long giant_step_val = power(a, k, m);
    current_val = giant_step_val;

    for (long long i = 1; i <= k; ++i) {
        // Check if the current giant step value is in our map
        if (baby_steps.count(current_val)) {
            long long j = baby_steps[current_val];
            // If a match is found, x = i*k - j
            return i * k - j;
        }
        current_val = (__int128)current_val * giant_step_val % m;
    }

    // If no solution is found after checking all steps
    return -1;
}


int main() {
    long long a, b, m;

    std::cout << "Solve for x in: a^x = b (mod m)" << std::endl;
    std::cout << "Enter base (a): ";
    std::cin >> a;
    std::cout << "Enter result (b): ";
    std::cin >> b;
    std::cout << "Enter modulus (m): ";
    std::cin >> m;

    long long result = discreteLog(a, b, m);

    if (result == -1) {
        std::cout << "No solution found." << std::endl;
    } else {
        std::cout << "The smallest non-negative solution for x is: " << result << std::endl;
        std::cout << "Verification: " << a << "^" << result << " (mod " << m << ") = " << power(a, result, m) << std::endl;
    }
    
    // Example from Wikipedia: 3^x = 13 (mod 17) -> x=4
    std::cout << "\n--- Example: 3^x = 13 (mod 17) ---" << std::endl;
    result = discreteLog(3, 13, 17);
    std::cout << "Result: x = " << result << std::endl; // Expected: 4

    return 0;
}
