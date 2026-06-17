#include "mitm_attack.h"
#include "math_utils.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

struct TableRow {
    int key;
    int value;
};

static int encrypt_simple(int text, int key, int mod) {
    return (text + key) % mod;
}

static int decrypt_simple(int cipher, int key, int mod) {
    return (cipher - key + mod) % mod;
}

void run_mitm_attack(ostream& out) {
    int mod = 16;

    int message = 7;

    int real_key1 = 5;
    int real_key2 = 9;

    int middle_value = encrypt_simple(message, real_key1, mod);
    int cipher = encrypt_simple(middle_value, real_key2, mod);

    vector<TableRow> left_table;
    vector<TableRow> right_table;

    out << "5\n";
    out << "\nАтака посередине\n";

    out << "\nИдея атаки:\n";
    out << "Пусть сообщение шифруется два раза:\n";
    out << "C = E(k2, E(k1, M))\n";
    out << "Если перебирать все пары ключей k1 и k2 напрямую, вариантов будет много.\n";
    out << "Атака посередине работает проще: мы идём к промежуточному значению с двух сторон.\n";

    out << "\nУчебный шифр для симуляции:\n";
    out << "E(k, x) = (x + k) mod " << mod << "\n";
    out << "D(k, x) = (x - k) mod " << mod << "\n";

    out << "\nДано:\n";
    out << "Открытый текст M = " << message << "\n";
    out << "Первый ключ k1 = " << real_key1 << "\n";
    out << "Второй ключ k2 = " << real_key2 << "\n";

    out << "\nСначала получим шифртекст:\n";
    out << "E(" << real_key1 << ", " << message << ") = "
        << middle_value << "\n";

    out << "E(" << real_key2 << ", " << middle_value << ") = "
        << cipher << "\n";

    out << "Итоговый шифртекст C = " << cipher << "\n";

    out << "\nШаг 1. Идём слева направо от открытого текста.\n";
    out << "Для каждого возможного k1 считаем E(k1, M):\n";

    for (int k1 = 0; k1 < mod; k1++) {
        int value = encrypt_simple(message, k1, mod);

        left_table.push_back({k1, value});

        out << "k1 = " << k1
            << ": E(" << k1 << ", " << message << ") = "
            << value << "\n";
    }

    out << "\nШаг 2. Идём справа налево от шифртекста.\n";
    out << "Для каждого возможного k2 считаем D(k2, C):\n";

    for (int k2 = 0; k2 < mod; k2++) {
        int value = decrypt_simple(cipher, k2, mod);

        right_table.push_back({k2, value});

        out << "k2 = " << k2
            << ": D(" << k2 << ", " << cipher << ") = "
            << value << "\n";
    }

    out << "\nШаг 3. Ищем одинаковые значения в двух таблицах.\n";
    out << "Если значения совпали, значит мы встретились посередине.\n\n";

    bool found = false;

    for (int i = 0; i < (int)left_table.size(); i++) {
        for (int j = 0; j < (int)right_table.size(); j++) {
            if (left_table[i].value == right_table[j].value) {
                found = true;

                int found_key1 = left_table[i].key;
                int found_key2 = right_table[j].key;
                int found_middle = left_table[i].value;

                out << "Совпадение найдено:\n";
                out << "Промежуточное значение = " << found_middle << "\n";
                out << "Возможная пара ключей: k1 = "
                    << found_key1 << ", k2 = " << found_key2 << "\n";

                int check_middle = encrypt_simple(message, found_key1, mod);
                int check_cipher = encrypt_simple(check_middle, found_key2, mod);

                out << "Проверка:\n";
                out << "E(" << found_key1 << ", " << message << ") = "
                    << check_middle << "\n";

                out << "E(" << found_key2 << ", " << check_middle << ") = "
                    << check_cipher << "\n";

                if (check_cipher == cipher) {
                    out << "Пара подходит, потому что получился нужный шифртекст C = "
                        << cipher << ".\n";
                } else {
                    out << "Пара не подходит.\n";
                }

                out << "\n";
            }
        }
    }

    if (!found) {
        out << "Совпадений не найдено.\n";
    }

    out << "Вывод:\n";
    out << "Атака посередине позволяет не перебирать все пары ключей сразу.\n";
    out << "Мы отдельно считаем значения от открытого текста и отдельно от шифртекста,\n";
    out << "а потом ищем совпадение промежуточного результата.\n";
}
