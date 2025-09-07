#include <iostream>
#include <tuple> // For returning multiple values

/**
 * @brief Computes the greatest common divisor of a and b, and finds coefficients x and y
 * such that ax + by = gcd(a, b).
 * * @param a The first integer.
 * @param b The second integer.
 * @param x Reference to an integer to store the coefficient of a.
 * @param y Reference to an integer to store the coefficient of b.
 * @return The greatest common divisor of a and b.
 */

 // for calculating inverse even if m is not prime.
 // ax=1 mod m.
 // ax+my=1 mod m
 // ecgcd(a,m)
 // it gives ax1+my1=gcd(a,m)
 // x1*m/gcd(a,m)=x;
 // this is inverse of a
 
long long extendedGcd(long long a, long long b, long long &x, long long &y) {
    // Base case of the recursion
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    // Variables to store the results of the recursive call
    long long x1, y1; 
    
    // Recursive call
    long long gcd = extendedGcd(b, a % b, x1, y1);

    // Update x and y using the results from the recursive call
    // x = y1
    // y = x1 - (a / b) * y1
    x = y1;
    y = x1 - (a / b) * y1;

    return gcd;
}

// --- How to Use It ---
int main() {
    long long a = 55;
    long long b = 80;
    long long x, y;

    long long g = extendedGcd(a, b, x, y);

    std::cout << "GCD(" << a << ", " << b << ") = " << g << std::endl;
    std::cout << "Coefficients (x, y): " << x << ", " << y << std::endl;
    std::cout << "Verification: " << a << "*" << x << " + " << b << "*" << y << " = " << a * x + b * y << std::endl;
    
    std::cout << "\n--- Another Example ---\n";
    
    a = 240;
    b = 46;
    g = extendedGcd(a, b, x, y);

    std::cout << "GCD(" << a << ", " << b << ") = " << g << std::endl;
    std::cout << "Coefficients (x, y): " << x << ", " << y << std::endl;
    std::cout << "Verification: " << a << "*" << x << " + " << b << "*" << y << " = " << a * x + b * y << std::endl;

    return 0;
}