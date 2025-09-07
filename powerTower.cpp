#include <iostream>
#include <vector>
#include <numeric>

// Struct to return both the value and a comparison flag.
struct Result {
    long long value; // The result modulo m
    bool is_ge;      // True if the actual value was >= m
};

// Calculates Euler's totient function phi(n)
int phi(int n) {
    int result = n;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            while (n % i == 0)
                n /= i;
            result -= result / i;
        }
    }
    if (n > 1)
        result -= result / n;
    return result;
}

// A custom power function that computes (base^exp) % mod and also checks
// if the actual value of base^exp is >= mod.
Result power(long long base, long long exp, int mod) {
    if (mod == 1) return {0, true};
    if (exp == 0) return {1 % mod, 1 >= mod};

    // Recursively compute (base^(exp/2))
    Result half = power(base, exp / 2, mod);
    Result res_sq;

    // Square the result of the half-exponent
    res_sq.value = (static_cast<__int128>(half.value) * half.value) % mod;
    
    // Check if half.value * half.value >= mod without overflow
    // The check a >= (m + b - 1) / b is a safe way to test for a * b >= m
    res_sq.is_ge = half.is_ge || (half.value > 0 && half.value >= (mod + half.value - 1) / half.value);

    if (exp % 2 == 0) {
        // If exp is even, result is just the square
        return res_sq;
    } else {
        // If exp is odd, multiply by base one more time
        Result final_res;
        long long base_mod = base % mod;
        
        final_res.value = (static_cast<__int128>(res_sq.value) * base_mod) % mod;
        
        // Check for final_res.value >= mod
        final_res.is_ge = res_sq.is_ge || (base >= mod) || (base > 0 && res_sq.value >= (mod + base - 1) / base);
        
        return final_res;
    }
}

// Global vector to store the tower numbers
std::vector<int> a;
int n;

// The main recursive function to solve the power tower
Result solve(int level, int mod) {
    // Base case: If mod is 1, the result of any x mod 1 is 0.
    if (mod == 1) {
        return {0, true};
    }
    // Base case: Reached the top of the tower.
    if (level == n - 1) {
        return {a[level] % mod, (long long)a[level] >= mod};
    }

    // Calculate phi(mod) for the exponent's modulus
    int phi_m = phi(mod);
    
    // Recursively solve for the exponent tower
    Result exp_res = solve(level + 1, phi_m);
    
    long long exponent = exp_res.value;
    // Apply Extended Euler's Theorem
    if (exp_res.is_ge) {
        exponent += phi_m;
    }
    
    // Calculate the final result with the determined exponent
    return power(a[level], exponent, mod);
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    int m;
    std::cin >> n >> m;
    
    a.resize(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }
    
    if (m == 1 && n > 0) {
        std::cout << 0 << std::endl;
        return 0;
    }
    
    Result final_result = solve(0, m);
    std::cout << final_result.value << std::endl;
    
    return 0;
}