#include <iostream>
#include <string>
#include <locale>
#include <fstream>
using namespace std;

// Функция для проверки наличия пятерки в записи числа
bool hasNoFive(int num) {
    string strNum = to_string(num);
    for (char digit : strNum) {
        if (digit == '5') {
            return false;
        }
    }
    return true;
}

// Функция для подсчета чисел, меньших n, без пятерок
int countNumbersWithoutFive(int n) {
    int count = 0;
    for (int i = 1; i < n; ++i) {
        if (hasNoFive(i)) {
            count++;
        }
    }
    return count;
}

int main() {
    setlocale(LC_ALL, "Russian");
    ofstream out("dots.txt");
    int n;
    cout << "value n: ";
    cin >> n; // Задаем значение n // Пример: подсчет для чисел меньше 100
    double seconds = 0;
    while (seconds < 20) {
        clock_t start = clock();
        int result = countNumbersWithoutFive(n);
        clock_t end = clock();
        cout << "Количество чисел меньше " << n << ", в записи которых нет пятерок: " << result << endl;
        seconds = (double)(end - start) / CLOCKS_PER_SEC;
        printf("The time: %f seconds\n", seconds);
        out << n << " " << seconds << endl;
        n *= 1.5;
    }
    return 0;
}
