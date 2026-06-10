#include "rf_crypto_standards.h"
#include "text_file.h"

#include <iostream>

void run_rf_crypto_standards(std::ostream& out) {
    out << "7\n";
    print_text_file("data/rf_crypto_standards.txt", out);
}
