#include <iostream>
#include <cmath>
#include <map>
#include <numeric> // For std::gcd in C++17
#include <vector>

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
 * @brief Computes gcd(a, b) and finds coefficients x, y such that ax + by = gcd(a, b).
 */
long long extendedGcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long x1, y1;
    long long gcd = extendedGcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return gcd;
}

/**
 * @brief Finds the modular multiplicative inverse of a under modulo m.
 * * It returns a value x such that (a * x) % m = 1.
 */
long long modInverse(long long a, long long m) {
    long long x, y;
    long long g = extendedGcd(a, m, x, y);
    if (g != 1) {
        // Inverse doesn't exist
        return -1;
    }
    // ensure x is positive
    return (x % m + m) % m;
}

/**
 * @brief Solves a system of congruences using the Chinese Remainder Theorem.
 * * Finds x such that:
 * * x ≡ remainders[0] (mod moduli[0])
 * * ...
 * * Assumes all moduli are pairwise coprime.
 * @param remainders A vector of remainders.
 * @param moduli A vector of moduli.
 * @return The smallest non-negative solution for x, or -1 on error.
 */
long long chineseRemainderTheorem(const std::vector<long long>& remainders, const std::vector<long long>& moduli) {
    if (remainders.size() != moduli.size() || remainders.empty()) {
        return -1; // Invalid input
    }

    long long N = 1;
    for (long long n : moduli) {
        N *= n;
    }

    long long result = 0;
    for (size_t i = 0; i < remainders.size(); ++i) {
        long long a_i = remainders[i];
        long long n_i = moduli[i];
        long long N_i = N / n_i;
        long long M_i = modInverse(N_i, n_i);

        if (M_i == -1) return -1; // Error: moduli not pairwise coprime

        result = (result + (__int128)a_i * N_i * M_i) % N;
    }

    return (result + N) % N; // Ensure result is positive
}




int main() {
    long long a, b, m;

    std::cout << "--- Discrete Logarithm Solver ---" << std::endl;
    std::cout << "Solve for x in: a^x = b (mod m)" << std::endl;
    std::cout << "Enter base (a): ";
    std::cin >> a;
    std::cout << "Enter result (b): ";
    std::cin >> b;
    std::cout << "Enter modulus (m): ";
    std::cin >> m;


    std::vector<long long> remainders = {2, 3, 2};
    std::vector<long long> moduli = {3, 5, 7};
    
    long long crt_result = chineseRemainderTheorem(remainders, moduli);

    if (crt_result == -1) {
        std::cout << "Could not solve the system (moduli might not be coprime)." << std::endl;
    } else {
        std::cout << "The smallest non-negative solution is x = " << crt_result << std::endl; // Expected: 23
    }


    return 0;
}

