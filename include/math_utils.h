#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <iosfwd>
#include <vector>
#include <string>

struct EgcdRow {
    int r;
    int x;
    int y;
    int q;
};

int gcd_int(int a, int b);
bool is_prime(int n, std::ostream* log = nullptr);
int normalize_mod(int a, int mod);
int mod_pow_simple(int a, int x, int mod, std::ostream& log);
int mod_pow_fermat(int a, int x, int p, std::ostream& log);
int mod_pow_binary(int a, int x, int mod, std::ostream& log);
std::vector<EgcdRow> extended_gcd_table(int a, int b, std::ostream& log);
int mod_inverse(int c, int m, std::ostream& log);
std::vector<int> continued_fraction(int n, int d, std::ostream& log);
bool solve_diophantine(int A, int B, int D,
                       int& x0, int& y0, int& g,
                       std::ostream& log);
void print_egcd_table(const std::vector<EgcdRow>& rows, std::ostream& log);

#endif
