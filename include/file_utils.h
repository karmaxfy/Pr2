#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <vector>

std::vector<unsigned char> read_binary_file(const std::string& path);
void write_binary_file(const std::string& path, const std::vector<unsigned char>& data);
void write_numbers_file(const std::string& path, const std::vector<int>& numbers);
std::vector<int> read_numbers_file(const std::string& path);

#endif
