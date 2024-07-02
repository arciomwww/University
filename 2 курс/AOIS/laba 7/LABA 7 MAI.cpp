#include <iostream>
#include <vector>
#include <cmath>
#include <bitset>
#include <string>
#include <cstdlib> 
#include <ctime>   
#include <locale>
#include <tuple>

using namespace std;


struct CacheLine {
    bool is_empty;
    int tag;
    vector<int> data;

    CacheLine(int block_size) : is_empty(true), tag(-1), data(block_size, -1) {}
};

// Функция для получения тега и смещения из адреса
tuple<int, int, int> get_tag_block_offset(int address, int block_size, int num_sets) {
    int offset_len = static_cast<int>(ceil(log2(block_size)));
    int block_len = static_cast<int>(ceil(log2(num_sets)));
    int offset_mask = (1 << offset_len) - 1;
    int block_mask = (1 << block_len) - 1;

    int offset = address & offset_mask;
    int block = (address >> offset_len) & block_mask;
    int tag = address >> (offset_len + block_len);

    return { tag, block, offset };
}


vector<int> create_ram(int size) {
    return vector<int>(size, 0);
}


int ram_read(const vector<int>& ram, int address) {
    cout << "Чтение из ОЗУ: адрес=" << address << endl;
    return ram[address];
}


void ram_write(vector<int>& ram, int address, int data) {
    cout << "Запись в ОЗУ: адрес=" << address << ", данные=" << data << endl;
    ram[address] = data;
}


int cache_line_read(const CacheLine& cache_line, int offset) {
    return cache_line.data[offset];
}


void cache_line_write(CacheLine& cache_line, int tag, int offset, int data) {
    cache_line.is_empty = false;
    cache_line.tag = tag;
    cache_line.data[offset] = data;
}


void shift(vector<CacheLine>& cache_lines, int index) {
    CacheLine temp = cache_lines[index];
    for (int i = index; i < cache_lines.size() - 1; ++i) {
        cache_lines[i] = cache_lines[i + 1];
    }
    cache_lines.back() = temp;
}


void cache_write(vector<CacheLine>& cache_lines, int address, int block_size, int num_sets, vector<int>& ram, int data) {
    auto tag_block_offset = get_tag_block_offset(address, block_size, num_sets);
    int tag = get<0>(tag_block_offset);
    int block = get<1>(tag_block_offset);
    int offset = get<2>(tag_block_offset);

    string binary_tag = bitset<3>(tag).to_string();
    string binary_block = bitset<3>(block).to_string();
    string binary_offset = bitset<2>(offset).to_string();
    string binary_data = bitset<32>(data).to_string();

    // Поиск строки кэша с соответствующим тегом
    for (auto& cache_line : cache_lines) {
        if (!cache_line.is_empty && cache_line.tag == tag) {
            if (cache_line.data[offset] != data) {
                cache_line_write(cache_line, tag, offset, data);
                cout << "Обновление записи в кэше: \"тег\":" << binary_tag << " \"блок\":" << binary_block << " \"смещение\":" << binary_offset << " \"данные\":" << binary_data << endl;
            }
            return;
        }
    }

    // Поиск пустой строки кэша
    for (auto& cache_line : cache_lines) {
        if (cache_line.is_empty) {
            cache_line_write(cache_line, tag, offset, data);
            cout << "Запись в пустую строку кэша: \"тег\":" << binary_tag << " \"блок\":" << binary_block << " \"смещение\":" << binary_offset << " \"данные\":" << binary_data << endl;
            return;
        }
    }

    // Если кэш полностью занят, выполняется замена строки
    shift(cache_lines, 0);
    cache_line_write(cache_lines.back(), tag, offset, data);
    cout << "Запись с заменой строки в кэше: \"тег\":" << binary_tag << " \"блок\":" << binary_block << " \"смещение\":" << binary_offset << " \"данные\":" << binary_data << endl;
}

// Функция для доступа к кэшу
int cache_access(vector<CacheLine>& cache_lines, int address, int block_size, int num_sets, vector<int>& ram) {
    auto tag_block_offset = get_tag_block_offset(address, block_size, num_sets);
    int tag = get<0>(tag_block_offset);
    int block = get<1>(tag_block_offset);
    int offset = get<2>(tag_block_offset);

    string binary_tag = bitset<3>(tag).to_string();
    string binary_block = bitset<3>(block).to_string();
    string binary_offset = bitset<2>(offset).to_string();

    // Поиск данных в кэше
    for (int i = 0; i < cache_lines.size(); ++i) {
        if (!cache_lines[i].is_empty && cache_lines[i].tag == tag) {
            int data = cache_line_read(cache_lines[i], offset);
            if (data != -1) {
                shift(cache_lines, i);
                string binary_data = bitset<32>(data).to_string();
                cout << "Попадание в кэш: \"тег\":" << binary_tag << "(" << tag << ") \"блок\":" << binary_block << "(" << block << ") \"смещение\":" << binary_offset << "(" << offset << ") \"данные\":" << binary_data << " (" << data << ")" << endl;
                return data;
            }
        }
    }

    // Если данных нет в кэше, выполняется чтение из ОЗУ
    int data = ram_read(ram, address);
    cache_write(cache_lines, address, block_size, num_sets, ram, data);
    string binary_data = bitset<32>(data).to_string();
    cout << "Промах в кэше: \"тег\":" << binary_tag << "(" << tag << ") \"блок\":" << binary_block << "(" << block << ") \"смещение\":" << binary_offset << "(" << offset << ") \"данные\":" << binary_data << " (" << data << ")" << endl;
    return data;
}

// Функция для отображения текущего состояния кэша
void show_cache(const vector<CacheLine>& cache_lines) {
    cout << "Индекс\tТег\tДанные" << endl;
    for (int i = 0; i < cache_lines.size(); ++i) {
        cout << i << "\t" << (cache_lines[i].tag != -1 ? to_string(cache_lines[i].tag) : "None") << "\t";
        for (int data : cache_lines[i].data) {
            cout << (data != -1 ? to_string(data) : "None") << "\t";
        }
        cout << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned>(time(0))); 

    vector<int> ram = create_ram(2048);
    vector<CacheLine> cache_lines(8, CacheLine(4));

    // Заполнение ОЗУ случайными данными
    for (int i = 0; i < 65; ++i) {
        ram_write(ram, i, rand() % 51);
    }

    while (true) {
        string input;
        cout << "Введите адрес: ";
        cin >> input;

        try {
            int address = stoi(input);
            cache_access(cache_lines, address, 4, 4, ram);
        }
        catch (invalid_argument&) {
            show_cache(cache_lines);
        }
    }

    return 0;
}
