#include "mitm_attack.h"
#include "text_file.h"

#include <iostream>

void run_mitm_attack(std::ostream& out) {
    out << "5\n";
    print_text_file("data/mitm_attack.txt", out);
}
