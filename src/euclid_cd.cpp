#include "euclid_cd.h"
#include "math_utils.h"

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

void run_euclid_cd(std::ostream& log) {
    int c = 11;
    int m = 13;

    std::ostringstream silent;
    std::vector<EgcdRow> rows = extended_gcd_table(m, c, silent);
    EgcdRow ans = rows[rows.size() - 2];
    int d = normalize_mod(ans.y, m);

    log << "Задание 2. Нахождение d из c*d mod m = 1\n";
    log << "Дано: c = " << c << ", m = " << m << "\n";
    log << "Чтобы найти d, решаем равенство Безу: m*u + c*v = НОД(m, c).\n";
    log << "Алгоритм Евклида:\n";
    print_euclid_divisions(rows, log);
    log << "НОД(" << m << ", " << c << ") = " << ans.r << ".\n";

    if (ans.r != 1) {
        log << "Так как НОД не равен 1, обратного элемента d не существует.\n";
        return;
    }

    log << "Расширенный алгоритм Евклида дал коэффициенты:\n";
    log << m << " * " << ans.x << " + " << c << " * " << ans.y << " = " << ans.r << ".\n";
    log << "Из этого следует: " << c << " * " << ans.y << " ≡ 1 (mod " << m << ").\n";
    log << "Приводим коэффициент " << ans.y << " по модулю " << m << ":\n";
    log << "d = " << ans.y << " mod " << m << " = " << d << ".\n";
    log << "Проверка: " << c << " * " << d << " mod " << m
        << " = " << (c * d) % m << "\n";
}
