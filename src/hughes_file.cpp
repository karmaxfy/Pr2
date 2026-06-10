#include "hughes_file.h"
#include "file_utils.h"
#include "math_utils.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

static int choose_session_key(int index) {
    return 17 + index * 2;
}

void run_hughes_file(std::ostream& log) {
    const std::string input_path = "data/input.txt";
    const std::string encrypted_path = "data/encrypted_hughes.txt";
    const std::string decrypted_path = "data/decrypted.txt";

    const int p = 331;
    const int g = 3;
    const int x = 37;

    std::ostringstream silent;
    int y = mod_pow_binary(g, x, p, silent);

    std::vector<unsigned char> input = read_binary_file(input_path);
    std::vector<int> encrypted;
    encrypted.reserve(input.size() * 2);

    for (int i = 0; i < static_cast<int>(input.size()); ++i) {
        int m = input[i];
        int k = choose_session_key(i);
        int c1 = mod_pow_binary(g, k, p, silent);
        int yk = mod_pow_binary(y, k, p, silent);
        int c2 = (m * yk) % p;
        encrypted.push_back(c1);
        encrypted.push_back(c2);
    }
    write_numbers_file(encrypted_path, encrypted);

    std::vector<int> encrypted_from_file = read_numbers_file(encrypted_path);
    std::vector<unsigned char> decrypted;
    decrypted.reserve(encrypted_from_file.size() / 2);

    int first_s = 0;
    int first_s_inv = 0;
    int first_m = 0;
    for (int i = 0; i + 1 < static_cast<int>(encrypted_from_file.size()); i += 2) {
        int c1 = encrypted_from_file[i];
        int c2 = encrypted_from_file[i + 1];
        int s = mod_pow_binary(c1, x, p, silent);
        int s_inv = mod_inverse(s, p, silent);
        int m = (c2 * s_inv) % p;
        if (i == 0) {
            first_s = s;
            first_s_inv = s_inv;
            first_m = m;
        }
        decrypted.push_back(static_cast<unsigned char>(m));
    }
    write_binary_file(decrypted_path, decrypted);

    log << "4\n";
    log << "Файл: " << input_path << "\n";
    log << "Параметры: p = " << p << ", g = " << g << ", закрытый ключ x = " << x << "\n";
    log << "Открытый ключ считается по формуле y = g^x mod p:\n";
    log << "y = " << g << "^" << x << " mod " << p << " = " << y << ".\n";

    if (!input.empty() && encrypted.size() >= 2) {
        int m0 = input[0];
        int k0 = choose_session_key(0);
        int c1 = encrypted[0];
        int c2 = encrypted[1];
        int yk = mod_pow_binary(y, k0, p, silent);

        log << "\nПокажем решение для первого байта файла.\n";
        log << "Первый байт: m = " << m0 << ".\n";
        log << "Берем сеансовое число k = " << k0 << ".\n";
        log << "Первая часть шифртекста:\n";
        log << "c1 = g^k mod p = " << g << "^" << k0 << " mod " << p << " = " << c1 << ".\n";
        log << "Вторая часть шифртекста:\n";
        log << "y^k mod p = " << y << "^" << k0 << " mod " << p << " = " << yk << ".\n";
        log << "c2 = m * y^k mod p = " << m0 << " * " << yk
            << " mod " << p << " = " << c2 << ".\n";
        log << "Значит первая зашифрованная пара: (" << c1 << ", " << c2 << ").\n";

        log << "\nРасшифрование первого байта:\n";
        log << "s = c1^x mod p = " << c1 << "^" << x << " mod " << p << " = " << first_s << ".\n";
        log << "s^(-1) mod p = " << first_s_inv << ".\n";
        log << "m = c2 * s^(-1) mod p = " << c2 << " * " << first_s_inv
            << " mod " << p << " = " << first_m << ".\n";
    }

    log << "\nЗашифровано пар: " << encrypted.size() / 2 << "\n";
    log << "Зашифрованный файл: " << encrypted_path << "\n";
    log << "Расшифрованный файл: " << decrypted_path << "\n";
    log << "Проверка: " << ((input == decrypted) ? "успешно" : "ошибка") << "\n";
}
