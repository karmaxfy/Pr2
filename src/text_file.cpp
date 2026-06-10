#include "text_file.h"

#include <fstream>
#include <iostream>
#include <string>

void print_text_file(const std::string& path, std::ostream& out) {
    std::ifstream file(path);
    if (!file.is_open()) {
        out << "Не удалось открыть файл: " << path << "\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        out << line << "\n";
    }
}
