#include "fraction_equation.h"
#include "math_utils.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>

static void print_euclid_divisions(const std::vector<EgcdRow>& rows, std::ostream& log) {
    for (int i = 2; i < static_cast<int>(rows.size()); ++i) {
        int left = rows[i - 2].r;
        int right = rows[i - 1].r;
        int q = rows[i].q;
        int r = rows[i].r;
        log << left << " = " << right << " * " << q << " + " << r << "\n";
    }
}

static int choose_small_t(int x0, int y0, int step_x, int step_y) {
    int best_t = 0;
    int best_score = std::abs(x0) + std::abs(y0);
    for (int t = -50; t <= 50; ++t) {
        int x = x0 + step_x * t;
        int y = y0 - step_y * t;
        int score = std::abs(x) + std::abs(y);
        if (score < best_score) {
            best_score = score;
            best_t = t;
        }
    }
    return best_t;
}

void run_fraction_equation(std::ostream& log) {
    const int A = 439;
    const int B = 118;
    const int D = 3;

    std::ostringstream silent;
    std::vector<int> cf = continued_fraction(A, B, silent);
    std::vector<EgcdRow> rows = extended_gcd_table(A, B, silent);
    EgcdRow ans = rows[rows.size() - 2];

    log << "6\n";
    log << "Вариант 5: " << A << "a + " << B << "b = " << D << "\n";
    log << "Алгоритм Евклида:\n";
    print_euclid_divisions(rows, log);

    log << A << "/" << B << " = [";
    for (int i = 0; i < static_cast<int>(cf.size()); ++i) {
        if (i != 0) log << "; ";
        log << cf[i];
    }
    log << "]\n";

    int g = ans.r;
    log << "НОД(" << A << ", " << B << ") = " << g << ".\n";
    if (D % g != 0) {
        log << "Решений нет, потому что " << D << " не делится на " << g << ".\n";
        return;
    }

    int k = D / g;
    int x0 = ans.x * k;
    int y0 = ans.y * k;
    int step_x = B / g;
    int step_y = A / g;
    int best_t = choose_small_t(x0, y0, step_x, step_y);
    int a = x0 + step_x * best_t;
    int b = y0 - step_y * best_t;

    log << "Расширенный алгоритм Евклида дал:\n";
    log << A << " * " << ans.x << " + " << B << " * " << ans.y << " = " << g << ".\n";
    log << "Умножаем коэффициенты на D / НОД = " << D << " / " << g << " = " << k << ":\n";
    log << A << " * " << x0 << " + " << B << " * " << y0 << " = " << D << ".\n";
    log << "Общее решение:\n";
    log << "a = " << x0 << " + " << step_x << "t\n";
    log << "b = " << y0 << " - " << step_y << "t\n";
    log << "Для более короткого ответа берем t = " << best_t << ":\n";
    log << "a = " << x0 << " + " << step_x << " * " << best_t << " = " << a << "\n";
    log << "b = " << y0 << " - " << step_y << " * " << best_t << " = " << b << "\n";
    log << "Проверка: " << A << " * " << a << " + " << B << " * " << b
        << " = " << A * a + B * b << "\n";
}
