#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <locale>
// Функция сжатия текста с использованием алгоритма LZW
std::vector<int> compress(const std::string& input) {
    std::unordered_map<std::string, int> dictionary;
    for (int i = 0; i < 256; ++i) {
        dictionary[std::string(1, i)] = i;
    }

    std::string current;
    std::vector<int> result;
    for (char c : input) {
        std::string combined = current + c;
        if (dictionary.find(combined) != dictionary.end()) {
            current = combined;
        }
        else {
            result.push_back(dictionary[current]);
            std::cout << current << ":" << dictionary[current] << std::endl;
            dictionary[combined] = dictionary.size();
            current = std::string(1, c);
        }
    }

    if (!current.empty()) {
        result.push_back(dictionary[current]);
    }

    return result;
}

// Функция декомпрессии текста с использованием алгоритма LZW
std::string decompress(const std::vector<int>& compressed) {
    std::unordered_map<int, std::string> dictionary;
    for (int i = 0; i < 256; ++i) {
        dictionary[i] = std::string(1, i);
    }

    std::string previous(1, compressed[0]);
    std::string result = previous;
    std::string current;
    for (int i = 1; i < compressed.size(); ++i) {
        int code = compressed[i];
        if (dictionary.find(code) != dictionary.end()) {
            current = dictionary[code];
        }
        else if (code == dictionary.size()) {
            current = previous + previous[0];
        }
        else {
            throw std::invalid_argument("Некорректные данные сжатия.");
        }

        result += current;
        dictionary[dictionary.size()] = previous + current[0];
        previous = current;
    }

    return result;
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::string original_text = "hello world";

    // Сжатие текста
    std::vector<int> compressed_text = compress(original_text);
    std::cout << "Сжатый текст: ";
    for (int code : compressed_text) {
        std::cout << code << " ";
    }
    std::cout << std::endl;

    // Декомпрессия текста
    std::string decompressed_text = decompress(compressed_text);
    std::cout << "Декомпрессированный текст: " << decompressed_text << std::endl;

    return 0;
}
