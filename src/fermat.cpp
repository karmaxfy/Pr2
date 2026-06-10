#include "fermat.h"
#include "math_utils.h"

#include <iostream>
#include <sstream>
#include <vector>

static int power_without_mod(int base, int exp) {
    int value = 1;
    for (int i = 0; i < exp; ++i) {
        value *= base;
    }
    return value;
}

static void explain_mod_power_steps(int base, int exp, int mod, std::ostream& log) {
    int result = 1;
    for (int i = 1; i <= exp; ++i) {
        int before = result;
        int product = before * base;
        result = product % mod;
        log << "Шаг " << i << ": " << before << " * " << base
            << " = " << product << ", " << product << " mod " << mod
            << " = " << result << "\n";
    }
}

void run_fermat(std::ostream& log) {
    int a = 23;
    int x = 3;
    int p = 5;

    std::ostringstream silent;
    int r_fermat = mod_pow_fermat(a, x, p, silent);
    int r_binary = mod_pow_binary(a, x, p, silent);
    int g = gcd_int(a, p);
    int reduced_base = normalize_mod(a, p);
    int reduced_power = x;

    log << "Задание 1. Вычисление a^x mod p\n";
    log << "Дано: " << a << "^" << x << " mod " << p << "\n";
    log << "Проверяем условия теоремы Ферма:\n";
    log << "p = " << p << (is_prime(p) ? " — простое число.\n" : " — не простое число.\n");
    log << "НОД(" << a << ", " << p << ") = " << g << ".\n";

    log << "Сначала уменьшаем основание по модулю:\n";
    log << a << " = " << p << " * " << (a / p) << " + " << reduced_base << ", значит "
        << a << " ≡ " << reduced_base << " (mod " << p << ").\n";

    if (is_prime(p) && g == 1) {
        reduced_power = x % (p - 1);
        log << "Условия теоремы Ферма выполняются.\n";
        log << "По теореме Ферма показатель можно заменить на x mod (p - 1):\n";
        log << x << " mod " << (p - 1) << " = " << reduced_power << ".\n";
    } else {
        log << "Условия теоремы Ферма не выполняются, поэтому степень не уменьшаем.\n";
    }

    int shown_power = power_without_mod(reduced_base, reduced_power);
    log << "Получаем: " << a << "^" << x << " mod " << p << " = "
        << reduced_base << "^" << reduced_power << " mod " << p << ".\n";
    log << reduced_base << "^" << reduced_power << " = " << shown_power << ".\n";
    log << shown_power << " = " << p << " * " << (shown_power / p)
        << " + " << (shown_power % p) << ".\n";
    log << "Ответ через теорему Ферма: " << r_fermat << "\n";

    log << "\nБинарный алгоритм:\n";
    log << "Степень " << x << " раскладывается по битам, а основание каждый шаг возводится в квадрат по модулю.\n";
    explain_mod_power_steps(reduced_base, x, p, log);
    log << "Ответ бинарным методом: " << r_binary << "\n";
}
