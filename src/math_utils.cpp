#include "math_utils.h"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stdexcept>

int gcd_int(int a, int b) {
    a = std::abs(a);
    b = std::abs(b);
    while (b != 0) {
        int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

bool is_prime(int n, std::ostream* log) {
    if (log) *log << "Проверка простоты числа p = " << n << "\n";
    if (n < 2) {
        if (log) *log << "Число меньше 2, значит не является простым.\n";
        return false;
    }
    if (n == 2) {
        if (log) *log << "2 - простое число.\n";
        return true;
    }
    if (n % 2 == 0) {
        if (log) *log << n << " делится на 2, значит число составное.\n";
        return false;
    }
    for (int i = 3; i * i <= n; i += 2) {
        if (log) *log << "Пробуем делитель " << i << ": ";
        if (n % i == 0) {
            if (log) *log << n << " делится на " << i << ", число составное.\n";
            return false;
        }
        if (log) *log << "не делится.\n";
    }
    if (log) *log << "Делители не найдены, число простое.\n";
    return true;
}

int normalize_mod(int a, int mod) {
    int r = a % mod;
    if (r < 0) r += mod;
    return r;
}

int mod_pow_simple(int a, int x, int mod, std::ostream& log) {
    int result = 1;
    a = normalize_mod(a, mod);
    log << "Обычное циклическое умножение: начальное значение result = 1, a mod p = " << a << "\n";
    for (int i = 1; i <= x; ++i) {
        result = (result * a) % mod;
        log << "Шаг " << i << ": result = result * a mod p = " << result << "\n";
    }
    return result;
}

int mod_pow_fermat(int a, int x, int p, std::ostream& log) {
    log << "\nАлгоритм 1. Вычисление через теорему Ферма\n";
    bool prime = is_prime(p, &log);
    int g = gcd_int(a, p);
    log << "НОД(" << a << ", " << p << ") = " << g << "\n";
    if (prime && g == 1) {
        int reduced_x = x % (p - 1);
        log << "Условия теоремы Ферма выполнены: p простое и НОД(a, p) = 1.\n";
        log << "По малой теореме Ферма: a^(p-1) ≡ 1 (mod p).\n";
        log << "Показатель степени можно уменьшить: x mod (p - 1) = "
            << x << " mod " << (p - 1) << " = " << reduced_x << "\n";
        return mod_pow_simple(a, reduced_x, p, log);
    }
    log << "Условия теоремы Ферма не выполнены, поэтому считаем напрямую циклом.\n";
    return mod_pow_simple(a, x, p, log);
}

int mod_pow_binary(int a, int x, int mod, std::ostream& log) {
    log << "\nАлгоритм 2. Бинарное возведение в степень\n";
    int result = 1;
    int base = normalize_mod(a, mod);
    int exp = x;
    log << "Начальные значения: result = 1, base = a mod p = " << base
        << ", exp = " << exp << "\n";
    int step = 1;
    while (exp > 0) {
        log << "Шаг " << step << ": exp = " << exp
            << ", младший бит = " << (exp & 1) << "\n";
        if (exp & 1) {
            result = (result * base) % mod;
            log << "  Бит равен 1: result = result * base mod p = " << result << "\n";
        } else {
            log << "  Бит равен 0: result не меняется.\n";
        }
        base = (base * base) % mod;
        exp >>= 1;
        log << "  base = base^2 mod p = " << base << ", exp = exp / 2 = " << exp << "\n";
        ++step;
    }
    log << "Ответ бинарным методом: " << result << "\n";
    return result;
}

void print_egcd_table(const std::vector<EgcdRow>& rows, std::ostream& log) {
    log << std::left << std::setw(12) << "r"
        << std::setw(12) << "x/u"
        << std::setw(12) << "y/v"
        << std::setw(12) << "q" << "\n";
    log << "------------------------------------------------\n";
    for (const EgcdRow& row : rows) {
        log << std::left << std::setw(12) << row.r
            << std::setw(12) << row.x
            << std::setw(12) << row.y;
        if (row.q >= 0) log << std::setw(12) << row.q;
        else log << std::setw(12) << "-";
        log << "\n";
    }
}

std::vector<EgcdRow> extended_gcd_table(int a, int b, std::ostream& log) {
    log << "\nРасширенный алгоритм Евклида для чисел " << a << " и " << b << "\n";
    std::vector<EgcdRow> rows;
    rows.push_back({a, 1, 0, -1});
    rows.push_back({b, 0, 1, -1});

    while (rows.back().r != 0) {
        EgcdRow prev = rows[rows.size() - 2];
        EgcdRow cur = rows[rows.size() - 1];
        int q = prev.r / cur.r;
        int r = prev.r - q * cur.r;
        int x = prev.x - q * cur.x;
        int y = prev.y - q * cur.y;
        log << prev.r << " = " << cur.r << " * " << q << " + " << r << "\n";
        log << "Новая строка: r = " << r << ", x = " << prev.x << " - " << q
            << " * " << cur.x << " = " << x << ", y = " << prev.y << " - " << q
            << " * " << cur.y << " = " << y << "\n";
        rows.push_back({r, x, y, q});
    }

    log << "\nТаблица вычислений:\n";
    print_egcd_table(rows, log);
    const EgcdRow& ans = rows[rows.size() - 2];
    log << "Предпоследняя строка содержит ответ: НОД = " << ans.r
        << ", u = " << ans.x << ", v = " << ans.y << "\n";
    log << a << " * " << ans.x << " + " << b << " * " << ans.y
        << " = " << ans.r << "\n";
    return rows;
}

int mod_inverse(int c, int m, std::ostream& log) {
    int normalized_c = normalize_mod(c, m);
    log << "\nПоиск обратного элемента: " << normalized_c << "^(-1) mod " << m << "\n";
    std::vector<EgcdRow> rows = extended_gcd_table(normalized_c, m, log);
    const EgcdRow& ans = rows[rows.size() - 2];
    if (ans.r != 1) {
        log << "НОД != 1, обратного элемента не существует.\n";
        return -1;
    }
    int inverse = normalize_mod(ans.x, m);
    log << "Коэффициент u = " << ans.x << ". Приводим его по модулю: "
        << inverse << "\n";
    log << "Проверка: " << normalized_c << " * " << inverse << " mod " << m
        << " = " << (normalized_c * inverse) % m << "\n";
    return inverse;
}

std::vector<int> continued_fraction(int n, int d, std::ostream& log) {
    log << "\nПредставление числа " << n << "/" << d << " в виде цепной дроби\n";
    std::vector<int> cf;
    while (d != 0) {
        int q = n / d;
        int r = n % d;
        log << n << " = " << d << " * " << q << " + " << r << "\n";
        cf.push_back(q);
        n = d;
        d = r;
    }
    log << "Цепная дробь: [";
    for (size_t i = 0; i < cf.size(); ++i) {
        if (i > 0) log << "; ";
        log << cf[i];
    }
    log << "]\n";
    return cf;
}

bool solve_diophantine(int A, int B, int D,
                       int& x0, int& y0, int& g,
                       std::ostream& log) {
    log << "\nРешение уравнения " << A << "a + " << B << "b = " << D << "\n";
    std::vector<EgcdRow> rows = extended_gcd_table(A, B, log);
    const EgcdRow& ans = rows[rows.size() - 2];
    g = ans.r;
    if (D % g != 0) {
        log << "D не делится на НОД(A, B), целых решений нет.\n";
        return false;
    }
    int k = D / g;
    x0 = ans.x * k;
    y0 = ans.y * k;
    log << "НОД(A, B) = " << g << ", множитель k = D / НОД = " << k << "\n";
    log << "Частное решение: a = " << x0 << ", b = " << y0 << "\n";
    log << "Проверка: " << A << " * " << x0 << " + " << B << " * " << y0
        << " = " << A * x0 + B * y0 << "\n";
    log << "Общее решение: a = " << x0 << " + " << (B / g)
        << "t, b = " << y0 << " - " << (A / g) << "t, где t - любое целое число.\n";
    return true;
}
