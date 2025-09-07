#include <iostream>
#include <vector>

/**
 * @brief Calculates (base^exp) % mod using binary exponentiation.
 * This is an efficient way to compute powers in modular arithmetic.
 * Time Complexity: O(log exp)
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
 * @brief Precomputes factorials and their modular inverses up to p.
 * Time Complexity: O(p)
 * @param fact Vector to store factorials modulo p.
 * @param invFact Vector to store modular inverse of factorials modulo p.
 * @param p The prime modulus.
 */
void precomputeFactorials(std::vector<long long>& fact, std::vector<long long>& invFact, int p) {
    fact[0] = 1;
    invFact[0] = 1;
    for (int i = 1; i < p; i++) {
        fact[i] = (fact[i - 1] * i) % p;
    }
    
    // Calculate inverse of (p-1)! using Fermat's Little Theorem
    invFact[p - 1] = power(fact[p - 1], p - 2, p);

    // Calculate other inverse factorials using the relation invFact[i-1] = invFact[i] * i
    for (int i = p - 2; i >= 1; i--) {
        invFact[i] = (invFact[i + 1] * (i + 1)) % p;
    }
}


/**
 * @brief Calculates nCr % p using precomputed factorials and their inverses.
 * This is the O(1) core calculator for Lucas's Theorem.
 * Time Complexity: O(1)
 */
long long nCr_mod_p(long long n, long long r, long long p, const std::vector<long long>& fact, const std::vector<long long>& invFact) {
    if (r < 0 || r > n) {
        return 0;
    }
    // Formula: n! * (r!)^-1 * ((n-r)!)^-1 mod p
    return (((__int128)fact[n] * invFact[r]) % p * invFact[n - r]) % p;
}

/**
 * @brief Computes nCr % p using Lucas's Theorem with precomputation.
 * This is highly efficient for large n and r but requires p to be a prime.
 * Time Complexity: O(log_p(n)) after an initial O(p) precomputation.
 * @param n The total number of items.
 * @param r The number of items to choose.
 * @param p A prime modulus.
 * @param fact Precomputed factorials modulo p.
 * @param invFact Precomputed inverse factorials modulo p.
 * @return The value of nCr mod p.
 */
long long lucasTheorem(long long n, long long r, long long p, const std::vector<long long>& fact, const std::vector<long long>& invFact) {
    // Base case
    if (r < 0 || r > n) {
        return 0;
    }
    
    long long result = 1;
    while (n > 0) {
        long long ni = n % p;
        long long ri = r % p;

        // If at any point ri > ni, then (ni C ri) is 0, making the whole product 0.
        if (ri > ni) {
            return 0;
        }

        // Calculate C(ni, ri) % p and multiply it into the result.
        result = (__int128)result * nCr_mod_p(ni, ri, p, fact, invFact) % p;

        // Move to the next digits in base p
        n /= p;
        r /= p;
    }
    return result;
}

int main() {
    long long n, r, p;

    std::cout << "--- Lucas's Theorem for nCr % p ---" << std::endl;
    std::cout << "Enter n: ";
    std::cin >> n;
    std::cout << "Enter r: ";
    std::cin >> r;
    std::cout << "Enter a prime modulus (p): ";
    std::cin >> p;

    if (p <= 0) {
        std::cout << "Error: Modulus must be a positive prime." << std::endl;
        return 1;
    }

    // Precomputation step
    std::vector<long long> fact(p);
    std::vector<long long> invFact(p);
    precomputeFactorials(fact, invFact, p);

    long long result = lucasTheorem(n, r, p, fact, invFact);
    std::cout << "Value of " << n << "C" << r << " mod " << p << " is: " << result << std::endl;

    // --- Example ---
    // C(1000, 100) mod 13
    // n=1000 -> 5 * 13^2 + 11 * 13 + 12 -> (5, 11, 12) in base 13
    // r=100  -> 0 * 13^2 + 7 * 13 + 9   -> (0, 7, 9) in base 13
    // Result = (C(5,0) * C(11,7) * C(12,9)) mod 13
    //        = (1 * 330 * 220) mod 13
    //        = (1 * 5 * 12) mod 13 = 60 mod 13 = 8
    std::cout << "\n--- Verification Example ---" << std::endl;
    std::cout << "C(1000, 100) mod 13 should be 8" << std::endl;
    std::vector<long long> fact_ex(13);
    std::vector<long long> invFact_ex(13);
    precomputeFactorials(fact_ex, invFact_ex, 13);
    std::cout << "Calculated result: " << lucasTheorem(1000, 100, 13, fact_ex, invFact_ex) << std::endl;

    return 0;
}

