#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Структура для представления записи в памяти
struct Record {
    string firstName;
    string lastName;
    string middleName;
};

// Функция для загрузки данных из файла в память
vector<Record> loadMemory(const string& filename) {
    vector<Record> memory;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка открытия файла." << endl;
        return memory;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Record record;
        ss >> record.firstName >> record.lastName >> record.middleName;
        memory.push_back(record);
    }

    file.close();
    return memory;
}

// Функция для вывода содержимого памяти на экран
void displayMemory(const vector<Record>& memory) {
    cout << "Содержимое памяти:" << endl;
    for (const auto& record : memory) {
        cout << record.firstName << "\t" << record.lastName << "\t" << record.middleName << endl;
    }
}

// Операция сравнения фамилий с учетом регистра маски
bool compareLastName(const string& a, const string& b, const string& mask, const string& op, int step) {
    // Определяем длину сравниваемых частей строк с учетом шага
    int length = mask.size() + step;

    // Обрезаем строки до нужной длины
    string aMasked = a.substr(0, length);
    string bMasked = b.substr(0, length);

    // Если длина строки больше длины маски, оставшиеся символы заменяются на 'X'.
    // Делаем для маскирования дополнительных символов и обеспечения сравнимости строк.
    if (length < a.size()) {
        for (int i = length; i < a.size(); ++i) {
            aMasked += 'X';
        }
    }

    if (length < b.size()) {
        for (int i = length; i < b.size(); ++i) {
            bMasked += 'X';
        }
    }

    cout << "Сравнение: " << aMasked << " " << op << " " << bMasked << endl;

    if (op == "=") {
        return aMasked == bMasked;
    }
    else if (op == ">") {
        return aMasked > bMasked;
    }
    else if (op == ">=") {
        return aMasked >= bMasked;
    }
    else if (op == "<") {
        return aMasked < bMasked;
    }
    else if (op == "<=") {
        return aMasked <= bMasked;
    }
    else if (op == "<>") {
        return aMasked != bMasked;
    }
    else {
        cerr << "Неподдерживаемая операция сравнения." << endl;
        return false;
    }
}

// Функция для поиска и сравнения записей в памяти
void searchAndCompare(const vector<Record>& memory, const string& searchKey, const string& mask, const string& compareOp) {
    cout << "Результаты поиска и сравнения:" << endl;
    for (const auto& record : memory) {
        cout << "Сравнение записи: " << record.lastName << " с " << searchKey << endl;

        // Начинаем сравнение с нулевого шага и увеличиваем шаг на каждом итерации
        for (int step = 0; step <= record.lastName.size() - mask.size(); ++step) {
            bool result = compareLastName(record.lastName, searchKey, mask, compareOp, step);
            cout << record.firstName << "\t" << record.lastName << "\t" << record.middleName << "\t";
            if (result) {
                cout << "Результат сравнения: true" << endl;
                break; // Если нашли совпадение, прерываем дальнейший поиск по тактам
            }
            else {
                cout << "Результат сравнения: false" << endl;
            }
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    // Загрузка данных из файла в память
    vector<Record> memory = loadMemory("data.txt");

    // Вывод содержимого памяти на экран
    displayMemory(memory);

    // Выполнение операции поиска и сравнения с использованием регистра маски
    string searchKey = "Smith";
    string mask = "Smit";
    string compareOp = ">=";
    searchAndCompare(memory, searchKey, mask, compareOp);

    return 0;
}
