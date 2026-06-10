#include "file_utils.h"

#include <fstream>
#include <stdexcept>

std::vector<unsigned char> read_binary_file(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("Не удалось открыть файл для чтения: " + path);
    return std::vector<unsigned char>((std::istreambuf_iterator<char>(in)),
                                      std::istreambuf_iterator<char>());
}

void write_binary_file(const std::string& path, const std::vector<unsigned char>& data) {
    std::ofstream out(path, std::ios::binary);
    if (!out) throw std::runtime_error("Не удалось открыть файл для записи: " + path);
    for (unsigned char ch : data) out.put(static_cast<char>(ch));
}

void write_numbers_file(const std::string& path, const std::vector<int>& numbers) {
    std::ofstream out(path);
    if (!out) throw std::runtime_error("Не удалось открыть файл для записи: " + path);
    for (int value : numbers) out << value << ' ';
}

std::vector<int> read_numbers_file(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("Не удалось открыть файл для чтения: " + path);
    std::vector<int> numbers;
    int value;
    while (in >> value) numbers.push_back(value);
    return numbers;
}
