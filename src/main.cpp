#include "fermat.h"
#include "euclid_cd.h"
#include "inverse.h"
#include "hughes_file.h"
#include "mitm_hughes.h"
#include "fraction_equation.h"
#include "message_tasks.h"

#include <iostream>
#include <stdexcept>
#include <string>

static void run_program(int number, std::ostream& out) {
    switch (number) {
        case 1: run_fermat(out); break;
        case 2: run_euclid_cd(out); break;
        case 3: run_inverse(out); break;
        case 4: run_hughes_file(out); break;
        case 5: run_mitm_message(out); break;
        case 6: run_fraction_equation(out); break;
        case 7: run_standards_rf_message(out); break;
        default: out << "Нет задания с таким номером.\n"; break;
    }
}

int main(int argc, char* argv[]) {
    try {
        int number = 0;
        if (argc >= 2) {
            number = std::stoi(argv[1]);
        } else {
            std::cout << "Практическая работа 2. Математические основы криптографии\n";
            std::cout << "Вариант 5: Хьюз, уравнение 439a + 118b = 3\n";
            std::cout << "1 - Ферма и бинарное возведение\n";
            std::cout << "2 - c*d mod m = 1 через u и v\n";
            std::cout << "3 - обратное число c^(-1) mod m\n";
            std::cout << "4 - Хьюз и шифрование файла\n";
            std::cout << "5 - сообщение: атака посередине\n";
            std::cout << "6 - цепная дробь и уравнение\n";
            std::cout << "7 - сообщение: стандарты криптографии в РФ\n";
            std::cout << "Номер задания: ";
            std::cin >> number;
        }
        run_program(number, std::cout);
    } catch (const std::exception& error) {
        std::cerr << "Ошибка: " << error.what() << "\n";
        return 1;
    }
    return 0;
}
