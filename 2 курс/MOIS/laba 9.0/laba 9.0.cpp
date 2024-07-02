#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <time.h>


using namespace std;

void generatePermutations(vector<int>& permutation, vector<bool>& chosen, int n) {
    if (permutation.size() == n) {
        // Выводим текущую перестановку
        for (int num : permutation) {
            cout << num << " ";
        }
        cout << endl;
    }
    else {
        for (int i = 1; i <= n; ++i) {
            if (!chosen[i]) {
                chosen[i] = true;
                permutation.push_back(i);
                generatePermutations(permutation, chosen, n);
                permutation.pop_back();
                chosen[i] = false;
            }
        }
    }
}

void generateAllPermutations(int n) {
    vector<int> permutation;
    vector<bool> chosen(n + 1, false);
    generatePermutations(permutation, chosen, n);
}

int main() {
    ofstream out("dots.txt");
    int n;
    cout << "value n: ";
    cin >> n; // Задаем значение n
    double seconds = 0;
    clock_t start = clock();
    generateAllPermutations(n);
    clock_t end = clock();
    seconds = (double)(end - start) / CLOCKS_PER_SEC;
    printf("The time: %f seconds\n", seconds);
    
  
    return 0;
}
