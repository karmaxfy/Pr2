#include "mitm_hughes.h"
#include "math_utils.h"

#include <iostream>
#include <sstream>
#include <string>

struct HughesPair {
    int c1;
    int c2;
};

static HughesPair encrypt_symbol(int m, int p, int g, int public_key, int k,
                                  std::ostream& silent) {
    int c1 = mod_pow_binary(g, k, p, silent);
    int yk = mod_pow_binary(public_key, k, p, silent);
    int c2 = (m * yk) % p;
    return {c1, c2};
}

static int decrypt_symbol(HughesPair pair, int p, int private_key,
                          std::ostream& silent) {
    int s = mod_pow_binary(pair.c1, private_key, p, silent);
    int s_inv = mod_inverse(s, p, silent);
    return (pair.c2 * s_inv) % p;
}

void run_mitm_hughes(std::ostream& log) {
    const int p = 331;
    const int g = 3;
    const int xAlice = 37;
    const int xEve = 53;
    const std::string message = "MITM";

    std::ostringstream silent;
    int yAlice = mod_pow_binary(g, xAlice, p, silent);
    int yEve = mod_pow_binary(g, xEve, p, silent);

    std::string eve_read;
    std::string alice_received;

    for (int i = 0; i < static_cast<int>(message.size()); ++i) {
        int m = static_cast<unsigned char>(message[i]);
        HughesPair toEve = encrypt_symbol(m, p, g, yEve, 19 + i * 2, silent);
        int opened = decrypt_symbol(toEve, p, xEve, silent);
        eve_read += static_cast<char>(opened);

        HughesPair toAlice = encrypt_symbol(opened, p, g, yAlice, 31 + i * 2, silent);
        int received = decrypt_symbol(toAlice, p, xAlice, silent);
        alice_received += static_cast<char>(received);
    }

    log << "5\n";//недоработан 
    log << "Сообщение Боба: " << message << "\n";
    log << "Ева подменила открытый ключ Алисы на свой.\n";
    log << "Ева прочитала: " << eve_read << "\n";
    log << "Алиса получила: " << alice_received << "\n";
    log << "Вывод: без аутентификации открытого ключа протокол уязвим.\n";
}
