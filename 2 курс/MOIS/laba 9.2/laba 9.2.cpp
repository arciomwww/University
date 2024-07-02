#include <iostream>
#include <vector>
#include <locale>
#include <fstream>

using namespace std;

vector<int> findPermutationFromInversions(const vector<int>& inversions) {
    int n = inversions.size();
    vector<int> permutation(n);

    for (int i = 0; i < n; ++i) {
        permutation[i] = n - inversions[i] - 1;
    }

    return permutation;
}

void printVector(const vector<int>& vec) {
    for (int num : vec) {
        cout << num << " ";
    }
    cout << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    ofstream out("dots.txt");  
    double seconds = 0;
    int n;
    cout << "value n: ";
    cin >> n; // Задаем значение n
    vector<int> inversions = { n-1, n-2, 0 }; // Пример таблицы инверсий
    clock_t start = clock();
    vector<int> permutation = findPermutationFromInversions(inversions);
    cout << "Полученная перестановка: ";
    printVector(permutation);
    clock_t end = clock();
    inversions.push_back(rand() % 10);
    seconds = (double)(end - start) / CLOCKS_PER_SEC;
    out << inversions.size() << " " << seconds << endl;
    printf("The time: %f seconds\n", seconds);

    return 0;
}
