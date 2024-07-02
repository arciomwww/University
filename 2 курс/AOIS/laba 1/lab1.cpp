#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;
struct Record {
    string name;
    string surname;
    string patronymic;
};

class HashTable {
private:

    const int TABLE_SIZE = 44;
    vector<vector<Record>> table;
    int collisionCount = 0;
    int checkSearch = 0;
    



public:
    HashTable() {
        table.resize(TABLE_SIZE);
    }
    const vector<vector<Record>>& getTable() const {
        return table;
    }

    int getCollisionCount() const {
        return collisionCount;
    }
    int getCheckSearch() const {
        return checkSearch;
    }

    int getTotalRecordsCount() const {
        int count = 0;
        for (const auto& bucket : table) {
            count += bucket.size();
        }
        return  count;
    }


    unsigned int hashFunction(const string& key) {
        int hash = 1;
        hash *= key[0]; // Берем первый символ
        hash *= key[2]; // Берем третий символ
        hash *= key[key.size() - 1]; // Берем последний символ
        double C = 0.618033988;
        double fk = fmod(C * hash, 1.0);
        unsigned int secondhash = floor(44 * fk);
        return secondhash % 44;
    }

    void resolveCollision(const Record& record, int initialIndex) {
        collisionCount++;
        int attempt = 0;
        int index = (initialIndex + attempt) % TABLE_SIZE; // используем линейное зондирование
        while (!table[index].empty()) {
            attempt++;
            index = (initialIndex + attempt) % TABLE_SIZE;
            if (attempt >= TABLE_SIZE) {
                return;
                cout << "n";
            }
        }
        table[index].push_back(record);
    }

    void addRecord(const Record& record) {
        if (getTotalRecordsCount() >= 44) {
            cout << "Таблица переполнена" << endl;
            return;
        }
        int index = hashFunction(record.name);
        if (!table[index].empty()) {
            resolveCollision(record, index); // использование линейного зондирования при коллизии
        }
        else {
            table[index].push_back(record);
        }
    }

    void removeRecord(const string& key) {
        int index = hashFunction(key);

        if (!table[index].empty()) {
            auto it = table[index].begin();
            while (it != table[index].end()) {
                if (it->name == key) {
                    it = table[index].erase(it);
                    return;
                }
                else {
                    ++it;
                }
            }
        }

        else {
            int counter = 0;
            bool found = false;
            while (counter < TABLE_SIZE - 1) {
                index = (index + 1) % TABLE_SIZE;
                counter++;
                for (auto it = table[index].begin(); it != table[index].end(); ++it) {
                    if (it->name == key) {
                        table[index].erase(it);
                        found = true;
                        break;
                    }
                }
                if (found) {
                    break;
                }
            }
        }
    }

    void modifyRecord(Record& record) {
        int index = hashFunction(record.name);
        for (int attempt = 0; attempt < TABLE_SIZE; ++attempt) {
            int newIndex = (index + attempt * hashFunction(record.name)) % TABLE_SIZE;
            for (auto& rec : table[newIndex]) {
                if (rec.name == record.name) {
                    cout << "Имя: " << rec.name << endl;

                    char answer;
                    cout << "Хотите изменить имя? (y/n): ";
                    cin >> answer;
                    if (answer == 'y') {
                        string newName;
                        cout << "Введите новое имя: ";
                        cin >> newName;

                        Record modifiedRecord{ newName, rec.surname, rec.patronymic };

                        table[newIndex].erase(remove_if(table[newIndex].begin(), table[newIndex].end(),
                            [&](const Record& r) { return r.name == rec.name; }), table[newIndex].end());

                        int newIndex = hashFunction(newName);
                        table[newIndex].push_back(modifiedRecord);
                    }

                    cout << "Фамилия: " << rec.surname << endl;
                    cout << "Хотите изменить фамилию? (y/n): ";
                    cin >> answer;
                    if (answer == 'y') {
                        string newSurname;
                        cout << "Введите новую фамилию: ";
                        cin >> newSurname;
                        rec.surname = newSurname;
                    }

                    cout << "Отчество: " << rec.patronymic << endl;
                    cout << "Хотите изменить отчество? (y/n): ";
                    cin >> answer;
                    if (answer == 'y') {
                        string newPatronymic;
                        cout << "Введите новое отчество: ";
                        cin >> newPatronymic;
                        rec.patronymic = newPatronymic;
                    }

                    return;
                }
            }
        }
    }



    Record* searchRecord(string& key) {
        checkSearch++;
        int index = hashFunction(key); // Используем хэш-функцию для вычисления индекса
        if (table[index].empty()) {
            for (int attempt = 0; attempt < TABLE_SIZE; ++attempt) {
                int newIndex = (index + attempt) % TABLE_SIZE;
                if (!table[newIndex].empty()) {
                    for (auto& rec : table[newIndex]) {
                        if (rec.name == key) {
                            return &rec;
                        }
                    }
                }
            }
        }
        else {
            for (auto& rec : table[index]) {
                if (rec.name == key) {
                    return &rec;
                }
            }
        }
        return nullptr;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    HashTable hashTable;
    ifstream inputFile("records.txt");
    string name, surname, patronymic;
    while (inputFile >> name >> surname >> patronymic) {
        Record record{ name, surname, patronymic };
        hashTable.addRecord(record);
    }

    int choice;
    string key;

    do {
        cout << "=== Меню ===" << endl;
        cout << "1. Добавить запись" << endl;
        cout << "2. Удалить запись" << endl;
        cout << "3. Отредактировать запись" << endl;
        cout << "4. Поиск записи" << endl;
        cout << "5. Вывод таблицы" << endl;
        cout << "6. Выход" << endl;

        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string name, surname, patronymic;
            cout << "Введите имя: ";
            cin >> name;
            cout << "Введите фамилию: ";
            cin >> surname;
            cout << "Введите отчество: ";
            cin >> patronymic;
            Record record{ name, surname, patronymic };
            hashTable.addRecord(record);

            break;
        }
        case 2: {
            cout << "Enter key: ";
            cin >> key;
            hashTable.removeRecord(key);
            break;
        }
        case 3: {
            string key, surname, patronymic;
            cout << "Введите ключ записи: ";
            cin >> key;
            Record modifiedRecord{ key, surname, patronymic };
            hashTable.modifyRecord(modifiedRecord);
            break;
        }
        case 4: {
            cout << "Enter key: ";
            cin >> key;

            Record* foundRecord = hashTable.searchRecord(key);
            if (foundRecord) {
                cout << "Record found: "
                    << foundRecord->name << " "
                    << foundRecord->surname << " "
                    << foundRecord->patronymic << endl;
            }
            else {
                cout << "Record with key '" << key << "' not found." << endl;
            }
            break;
        }
        case 5: {
            cout << "Showing table:" << endl;
            const auto& hashTableTable = hashTable.getTable();

            for (const auto& bucket : hashTableTable) {
                for (const auto& record : bucket) {
                    cout << "Name: " << record.name << ", Surname: " << record.surname << ", Patronymic: " << record.patronymic << endl;
                }
            }
            break;
        }
        case 6: {

            int f = hashTable.getCollisionCount();
            int d = hashTable.getTotalRecordsCount();
            int u = hashTable.getCheckSearch();

            cout << "Вероятность возникновения коллизий: " << (double)f / d * 100 << "%" << endl;
            cout << "Среднее кол-во обращений: " << (double)u / d << endl;

            break;
            return 0;
        }
        }
    } while (choice != 6);
    return 0;
}