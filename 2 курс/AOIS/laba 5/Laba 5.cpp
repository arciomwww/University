#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <locale>

std::atomic<bool> dataReady(false);
std::vector<int> numbers;
int blockSize;

void generateNumbers() {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000; ++i) {
        numbers.push_back(rand() % 10000);
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Время генерации чисел: " << duration.count() << " микросекунд" << std::endl;
    dataReady.store(true, std::memory_order_release);
}

void writeToFile(const std::string& filename) {
    while (!dataReady.load(std::memory_order_acquire)) {} // Ждем, пока данные не будут готовы
    auto start = std::chrono::high_resolution_clock::now();
    std::ofstream file(filename);
    for (int i = 0; i < numbers.size(); ++i) {
        file << numbers[i] << " ";
        if ((i + 1) % blockSize == 0) {
            file << "\n";
        }
    }
    file.close();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Запись чисел в файл " << filename << ": " << duration.count() << " микросекунд" << std::endl;
}

void sortAndWrite(const std::string& filename) {
    while (!dataReady.load(std::memory_order_acquire)) {} // Ждем, пока данные не будут готовы
    auto start = std::chrono::high_resolution_clock::now();
    std::ofstream file(filename);
    for (int i = 0; i < numbers.size(); i += blockSize) {
        std::sort(numbers.begin() + i, numbers.begin() + i + blockSize);
        for (int j = i; j < i + blockSize && j < numbers.size(); ++j) {
            file << numbers[j] << " ";
        }
        file << "\n";
    }
    file.close();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Запись чисел в файл " << filename << ": " << duration.count() << " микросекунд" << std::endl;
}



int main() {
    setlocale(LC_ALL, "Russian");
    std::cout << "Введите размер блока: ";
    std::cin >> blockSize;

    std::thread t1(generateNumbers);
    std::thread t2(writeToFile, "numbers.txt");
    std::thread t3(sortAndWrite, "sorted_numbers.txt");

    t1.join();
    t2.join();
    t3.join();

    return 0;
}