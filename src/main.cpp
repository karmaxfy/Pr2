#include "fermat.h"
#include "euclid_cd.h"
#include "inverse.h"
#include "hughes_file.h"
#include "mitm_attack.h"
#include "fraction_equation.h"
#include "rf_crypto_standards.h"

#include <clocale>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

// Меню сделано через enum class, чтобы в switch не использовать непонятные числа.
enum class MenuItem {
    Exit = 0,
    Fermat = 1,
    Euclid = 2,
    Inverse = 3,
    HughesFile = 4,
    MitmAttack = 5,
    FractionEquation = 6,
    RussianCryptoStandards = 7
};

static void run_menu_item(MenuItem item) {
    switch (item) {
        case MenuItem::Fermat:
            run_fermat(cout);
            break;
        case MenuItem::Euclid:
            run_euclid_cd(cout);
            break;
        case MenuItem::Inverse:
            run_inverse(cout);
            break;
        case MenuItem::HughesFile:
            run_hughes_file(cout);
            break;
        case MenuItem::MitmAttack:
            run_mitm_attack(cout);
            break;
        case MenuItem::FractionEquation:
            run_fraction_equation(cout);
            break;
        case MenuItem::RussianCryptoStandards:
            run_rf_crypto_standards(cout);
            break;
        case MenuItem::Exit:
            break;
        default:
            cout << "Такого пункта меню нет.\n";
            break;
    }
}

static void print_menu() {
    cout << "\n";
    cout << "1. Теорема Ферма и разложение степени в двоичный вид\n";
    cout << "2. Расширенный алгоритм Евклида для c*d mod m = 1\n";
    cout << "3. Обратный элемент по модулю\n";
    cout << "4. Протокол Хьюза для шифрования файла\n";
    cout << "5. Атака посередине\n";
    cout << "6. Цепная дробь и линейное уравнение\n";
    cout << "7. Стандарты современной криптографии в РФ\n";
    cout << "0. Выход\n";
    cout << "> ";
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");

    try {
        // Оставлено для make run1, make run2 и т.д.
        if (argc >= 2) {
            int punkt = stoi(argv[1]);
            run_menu_item(static_cast<MenuItem>(punkt));
            return 0;
        }

        while (true) {
            print_menu();

            int punkt;
            cin >> punkt;

            if (!cin) {
                cout << "Ошибка ввода.\n";
                return 1;
            }

            if (static_cast<MenuItem>(punkt) == MenuItem::Exit) {
                return 0;
            }

            run_menu_item(static_cast<MenuItem>(punkt));
        }
    } catch (const exception& error) {
        cerr << "Ошибка: " << error.what() << "\n";
        return 1;
    }
}
