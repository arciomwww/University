#include <iostream>
#include <vector>
#include <locale>
#include <time.h>

using namespace std;

vector<int> permutation_from_inversions(const vector<int>& inversions) {
    int n = inversions.size();
    vector<int> result(n);

    for (int i = 0; i < n; ++i) {
        result[n - 1 - inversions[i]] = i + 1;
    }

    return result;
}

int main() {
    setlocale(LC_ALL, "Russian");
    double seconds = 0;
    int n;
    cout << "Введите размер таблицы инверсий: ";
    cin >> n;

    vector<int> inversions(n);
    cout << "Введите элементы таблицы инверсий: ";
    for (int i = 0; i < n; ++i) {
        cin >> inversions[i];
    }
    clock_t start = clock();
    vector<int> permutation = permutation_from_inversions(inversions);
    clock_t end = clock();
    cout << "Полученная перестановка: ";
    for (int num : permutation) {
        cout << num << " ";
    }
    cout << endl;
    seconds = (double)(end - start) / CLOCKS_PER_SEC;
    printf("The time: %f seconds\n", seconds);

    return 0;
}
