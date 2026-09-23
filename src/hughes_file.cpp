#include "hughes_file.h"
#include "file_utils.h"
#include "math_utils.h"

#include <sstream>
#include <string>
#include <vector>

void run_hughes_file(std::ostream& log) {
    const std::string input_path = "data/input.txt";
    const std::string encrypted_path = "data/encrypted_hughes.txt";
    const std::string decrypted_path = "data/decrypted.txt";

    // Общие открытые параметры протокола Хьюза.
    const int p = 331;
    const int g = 3;

    // Секрет Алисы.
    const int x = 37;

    // Секрет Боба. Важно: НОД(y, p - 1) должен быть равен 1,
    // чтобы существовал обратный показатель z = y^(-1) mod (p - 1).
    const int y = 17;

    std::ostringstream silent;

    log << "4\n";
    log << "Протокол Хьюза и шифрование файла\n";
    log << "Файл: " << input_path << "\n";
    log << "Открытые параметры: p = " << p << ", g = " << g << ".\n";
    log << "Секрет Алисы x = " << x << ", секрет Боба y = " << y << ".\n\n";

    // 1. Алиса генерирует секретный ключ K = g^x mod p.
    int key_alice = mod_pow_binary(g, x, p, silent);
    log << "1) Алиса генерирует ключ:\n";
    log << "K = g^x mod p = " << g << "^" << x << " mod " << p
        << " = " << key_alice << ".\n\n";

    // 2. Боб выбирает y, взаимно простое с p - 1,
    // и отправляет Алисе Y = g^y mod p.
    int gcd_y = gcd_int(y, p - 1);
    log << "2) Проверяем показатель Боба:\n";
    log << "НОД(" << y << ", " << p - 1 << ") = " << gcd_y << ".\n";

    if (gcd_y != 1) {
        log << "Показатель y не подходит: обратного элемента по модулю p - 1 нет.\n";
        return;
    }

    int Y = mod_pow_binary(g, y, p, silent);
    log << "Боб вычисляет и отправляет Алисе:\n";
    log << "Y = g^y mod p = " << g << "^" << y << " mod " << p
        << " = " << Y << ".\n\n";

    // 3. Алиса возводит полученное Y в свою секретную степень x
    // и отправляет Бобу X = Y^x mod p.
    int X = mod_pow_binary(Y, x, p, silent);
    log << "3) Алиса вычисляет и отправляет Бобу:\n";
    log << "X = Y^x mod p = " << Y << "^" << x << " mod " << p
        << " = " << X << ".\n\n";

    // 4. Боб находит обратный показатель z к y по модулю p - 1.
    int z = mod_inverse(y, p - 1, silent);
    log << "4) Боб находит обратный показатель:\n";
    log << "z = y^(-1) mod (p - 1) = " << y << "^(-1) mod " << p - 1
        << " = " << z << ".\n";
    log << "Проверка: " << y << " * " << z << " mod " << p - 1
        << " = " << (y * z) % (p - 1) << ".\n\n";

    // 5. Боб восстанавливает ключ Алисы.
    int key_bob = mod_pow_binary(X, z, p, silent);
    log << "5) Боб восстанавливает общий ключ:\n";
    log << "K' = X^z mod p = " << X << "^" << z << " mod " << p
        << " = " << key_bob << ".\n";
    log << "Ключ Алисы K = " << key_alice
        << ", ключ Боба K' = " << key_bob << ".\n";

    if (key_alice != key_bob) {
        log << "Ошибка: ключи не совпали.\n";
        return;
    }

    log << "Ключи совпали. Протокол Хьюза выполнен успешно.\n\n";

    // После протокола обе стороны знают один и тот же ключ.
    // Для учебного шифрования файла используем умножение по модулю p:
    // c = m * K mod p.
    // Расшифрование: m = c * K^(-1) mod p.
    int key_inverse = mod_inverse(key_bob, p, silent);

    std::vector<unsigned char> input = read_binary_file(input_path);
    std::vector<int> encrypted;
    encrypted.reserve(input.size());

    for (int i = 0; i < static_cast<int>(input.size()); ++i) {
        int m = input[i];
        int c = (m * key_alice) % p;
        encrypted.push_back(c);
    }

    write_numbers_file(encrypted_path, encrypted);

    std::vector<int> encrypted_from_file = read_numbers_file(encrypted_path);
    std::vector<unsigned char> decrypted;
    decrypted.reserve(encrypted_from_file.size());

    for (int i = 0; i < static_cast<int>(encrypted_from_file.size()); ++i) {
        int c = encrypted_from_file[i];
        int m = (c * key_inverse) % p;
        decrypted.push_back(static_cast<unsigned char>(m));
    }

    write_binary_file(decrypted_path, decrypted);

    if (!input.empty() && !encrypted.empty()) {
        int m0 = input[0];
        int c0 = encrypted[0];
        int restored0 = (c0 * key_inverse) % p;

        log << "Шифрование первого байта файла:\n";
        log << "m = " << m0 << ".\n";
        log << "c = m * K mod p = " << m0 << " * " << key_alice
            << " mod " << p << " = " << c0 << ".\n\n";

        log << "Расшифрование первого байта:\n";
        log << "K^(-1) mod p = " << key_inverse << ".\n";
        log << "m = c * K^(-1) mod p = " << c0 << " * " << key_inverse
            << " mod " << p << " = " << restored0 << ".\n\n";
    }

    log << "Зашифровано байтов: " << encrypted.size() << "\n";
    log << "Зашифрованный файл: " << encrypted_path << "\n";
    log << "Расшифрованный файл: " << decrypted_path << "\n";
    log << "Проверка: " << ((input == decrypted) ? "успешно" : "ошибка") << "\n";
}
