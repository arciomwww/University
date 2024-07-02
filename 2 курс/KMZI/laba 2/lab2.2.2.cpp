#include <iostream>
#include <string>
#include <vector>
#include <locale>

// Функция для шифрования методом Виженера
std::string vigenere_encrypt(const std::string& plaintext, const std::string& key) {
    std::string ciphertext = "";
    int key_length = key.length();
    for (int i = 0; i < plaintext.length(); ++i) {
        char character = plaintext[i];
        if (isalpha(character)) {
            char key_char = key[i % key_length];
            int shift = toupper(key_char) - 'A';
            if (isupper(character)) {
                ciphertext += static_cast<char>((character + shift - 'A') % 26 + 'A');
            }
            else {
                ciphertext += static_cast<char>((character + shift - 'a') % 26 + 'a');
            }
        }
        else {
            ciphertext += character;
        }
    }
    return ciphertext;
}

// Функция для добавления заполнения согласно PKCS#7
std::string add_padding(const std::string& plaintext) {
    int pad_size = 8 - (plaintext.length() % 8);
    char pad_char = static_cast<char>(pad_size);
    std::string padded_text = plaintext;
    for (int i = 0; i < pad_size; ++i) {
        padded_text += pad_char;
    }
    return padded_text;
}

// Функция для удаления заполнения
std::string remove_padding(const std::string& padded_text) {
    char pad_char = padded_text[padded_text.length() - 1];
    int pad_size = static_cast<int>(pad_char);
    return padded_text.substr(0, padded_text.length() - pad_size);
}

// Функция для шифрования методом CBC
std::vector<std::string> cbc_encrypt(const std::string& plaintext, char key) {
    std::vector<std::string> blocks;
    std::string padded_text = add_padding(plaintext);
    std::string block = "";
    for (int i = 0; i < padded_text.length(); ++i) {
        char character = padded_text[i] ^ key;
        block += character;
        if (block.length() == 8 || i == padded_text.length() - 1) {
            blocks.push_back(vigenere_encrypt(block, std::string(1, key)));
            block = "";
        }
    }
    return blocks;
}

// Функция для расшифрования методом CBC
std::string cbc_decrypt(const std::vector<std::string>& blocks, char key) {
    std::string plaintext = "";
    std::string prev_block;
    for (const auto& block : blocks) {
        std::string decrypted_block = vigenere_encrypt(block, std::string(1, key));
        for (int i = 0; i < decrypted_block.length(); ++i) {
            char character = decrypted_block[i] ^ key;
            plaintext += character;
        }
    }
    return remove_padding(plaintext);
}

// Функция для расшифрования методом Виженера
std::string vigenere_decrypt(const std::string& ciphertext, const std::string& key) {
    std::string decrypted_text = "";
    int key_length = key.length();
    for (int i = 0; i < ciphertext.length(); ++i) {
        char character = ciphertext[i];
        if (isalpha(character)) {
            char key_char = key[i % key_length];
            int shift = toupper(key_char) - 'A';
            if (isupper(character)) {
                decrypted_text += static_cast<char>((character - shift - 'A' + 26) % 26 + 'A');
            }
            else {
                decrypted_text += static_cast<char>((character - shift - 'a' + 26) % 26 + 'a');
            }
        }
        else {
            decrypted_text += character;
        }
    }
    return decrypted_text;
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::string plaintext;
    char key;

    std::cout << "Введите открытый текст: ";
    std::getline(std::cin, plaintext);
    std::cout << "Введите ключевой байт: ";
    std::cin >> key;

    // Шифрование методом Виженера
    std::string vigenere_encrypted_text = vigenere_encrypt(plaintext, std::string(1, key));
    std::cout << "Зашифрованный текст методом Виженера: " << vigenere_encrypted_text << std::endl;

    // Шифрование методом CBC
    std::vector<std::string> cbc_blocks = cbc_encrypt(vigenere_encrypted_text, key);
    std::cout << "Зашифрованный текст методом CBC: ";
    for (const auto& block : cbc_blocks) {
        std::cout << block;
    }
    std::cout << std::endl;

    // Расшифрование текста методом CBC
    std::string decrypted_text = cbc_decrypt(cbc_blocks, key);
    std::cout << "Расшифрованный текст методом CBC: " << decrypted_text << std::endl;

    // Расшифрование текста методом Виженера
    std::string vigenere_decrypted_text = vigenere_decrypt(decrypted_text, std::string(1, key));
    std::cout << "Расшифрованный текст методом Виженера: " << vigenere_decrypted_text << std::endl;

    return 0;
}
