#include "inverse.h"
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

void run_inverse(std::ostream& log) {
    int c = 2;
    int m = 11;

    std::ostringstream silent;
    std::vector<EgcdRow> rows = extended_gcd_table(m, c, silent);
    EgcdRow ans = rows[rows.size() - 2];
    int inverse = normalize_mod(ans.y, m);

    log << "3\n";
    log << "Дано: c = " << c << ", m = " << m << "\n";
    log << "Нужно найти такое число d, что " << c << " * d ≡ 1 (mod " << m << ").\n";
    log << "Алгоритм Евклида:\n";
    print_euclid_divisions(rows, log);
    log << "НОД(" << m << ", " << c << ") = " << ans.r << ".\n";

    if (ans.r != 1) {
        log << "Обратного элемента нет, потому что НОД не равен 1.\n";
        return;
    }

    log << "Расширенный алгоритм Евклида дал равенство:\n";
    log << m << " * " << ans.x << " + " << c << " * " << ans.y << " = 1.\n";
    log << "Значит коэффициент при " << c << " является обратным по модулю " << m << ".\n";
    log << "c^(-1) mod m = " << ans.y << " mod " << m << " = " << inverse << ".\n";
    log << "Проверка: " << c << " * " << inverse << " mod " << m
        << " = " << (c * inverse) % m << "\n";
}
