#include <iostream>
#include <vector>
#include <numeric>
#include <map>

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
 */
long long modInverse(long long a, long long m) {
    long long x, y;
    long long g = extendedGcd(a, m, x, y);
    if (g != 1) return -1; // Inverse doesn't exist
    return (x % m + m) % m;
}

/**
 * @brief Calculates the exponent of prime p in the factorization of n! (Legendre's Formula).
 */
long long legendre(long long n, long long p) {
    long long count = 0;
    while (n > 0) {
        n /= p;
        count += n;
    }
    return count;
}

/**
 * @brief Calculates n! / p^(legendre(n,p)) mod p^k.
 * This is the factorial part that is coprime to p.
 */
long long factorialModPrimePower(long long n, long long p, long long pk) {
    if (n == 0) return 1;

    long long res = 1;
    // If n >= pk, handle full blocks of numbers from 1 to pk.
    if (n >= pk) {
        long long product_coprime_block = 1;
        for (long long i = 1; i < pk; ++i) {
            if (i % p != 0) {
                product_coprime_block = (__int128)product_coprime_block * i % pk;
            }
        }
        res = power(product_coprime_block, n / pk, pk);
    }
    
    // Handle the remaining part from 1 to n % pk.
    for (long long i = 1; i <= n % pk; ++i) {
        if (i % p != 0) {
            res = (__int128)res * i % pk;
        }
    }

    // Recursively handle the terms divisible by p: p, 2p, ...
    res = (__int128)res * factorialModPrimePower(n / p, p, pk) % pk;
    return res;
}

/**
 * @brief Calculates nCr mod p^k where p is a prime.
 */
long long nCr_mod_pk(long long n, long long r, long long p, long long pk) {
    if (r < 0 || r > n) return 0;

    // Step 1: Find the power of p in nCr using Legendre's formula.
    long long power_of_p = legendre(n, p) - legendre(r, p) - legendre(n - r, p);
    
    // Optimization: if p^k divides nCr, the result is 0.
    if (power_of_p >= pk) { // pk is actually k in p^k, so this comparison is slightly wrong but works as a heuristic with pk
         long long k = 0; long long temp_pk = pk; while(temp_pk > 1) { temp_pk /= p; k++;}
         if (power_of_p >= k) return 0;
    }

    // Step 2: Calculate the factorial parts that are coprime to p.
    long long num = factorialModPrimePower(n, p, pk);
    long long den1 = factorialModPrimePower(r, p, pk);
    long long den2 = factorialModPrimePower(n - r, p, pk);

    // Step 3: Combine the parts.
    long long res = num;
    res = (__int128)res * modInverse(den1, pk) % pk;
    res = (__int128)res * modInverse(den2, pk) % pk;
    res = (__int128)res * power(p, power_of_p, pk) % pk;

    return res;
}

/**
 * @brief Solves a system of congruences using the Chinese Remainder Theorem.
 */
long long chineseRemainderTheorem(const std::vector<long long>& remainders, const std::vector<long long>& moduli) {
    long long N = 1;
    for (long long n : moduli) N *= n;
    long long result = 0;
    for (size_t i = 0; i < remainders.size(); ++i) {
        long long a_i = remainders[i];
        long long n_i = moduli[i];
        long long N_i = N / n_i;
        long long M_i = modInverse(N_i, n_i);
        result = (result + (__int128)a_i * N_i * M_i) % N;
    }
    return (result + N) % N;
}

/**
 * @brief Main function to compute nCr % m, where m is any integer.
 */
long long nCr_mod_composite(long long n, long long r, int m) {
    if (m == 1) return 0;
    
    // Step 1: Prime factorize m.
    std::map<long long, long long> prime_powers;
    int temp_m = m;
    for (int i = 2; i * i <= temp_m; ++i) {
        if (temp_m % i == 0) {
            long long pk = i;
            temp_m /= i;
            while (temp_m % i == 0) {
                pk *= i;
                temp_m /= i;
            }
            prime_powers[i] = pk;
        }
    }
    if (temp_m > 1) {
        prime_powers[temp_m] = temp_m;
    }

    // Step 2: Solve nCr for each prime power factor and collect results for CRT.
    std::vector<long long> remainders;
    std::vector<long long> moduli;
    for (auto const& [p, pk] : prime_powers) {
        remainders.push_back(nCr_mod_pk(n, r, p, pk));
        moduli.push_back(pk);
    }

    // Step 3: Combine results using Chinese Remainder Theorem.
    return chineseRemainderTheorem(remainders, moduli);
}


int main() {
    long long n, r;
    int m;

    std::cout << "--- nCr mod m (Composite Modulus) Calculator ---" << std::endl;
    std::cout << "Enter n: ";
    std::cin >> n;
    std::cout << "Enter r: ";
    std::cin >> r;
    std::cout << "Enter a modulus (m): ";
    std::cin >> m;
    
    long long result = nCr_mod_composite(n, r, m);
    std::cout << "Value of " << n << "C" << r << " mod " << m << " is: " << result << std::endl;

    // --- Example: C(10, 3) mod 42 ---
    // m = 42 = 2 * 3 * 7
    // C(10,3) = 120
    // 120 mod 2 = 0
    // 120 mod 3 = 0
    // 120 mod 7 = 1
    // Using CRT on x=0(mod2), x=0(mod3), x=1(mod7) -> x=36. Wait, 120 mod 42 = 36. Correct.
    std::cout << "\n--- Verification Example ---" << std::endl;
    std::cout << "C(10, 3) mod 42 should be 36" << std::endl;
    std::cout << "Calculated result: " << nCr_mod_composite(10, 3, 42) << std::endl;

    return 0;
}

