#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <fstream>
using namespace std;
#define INF 9999999

bool gen(vector<int>& a, int n) {
    int j = a.size() - 2;
    while (j != -1 && a[j] > a[j + 1]) j--;
    if (j == -1) return false;
    int k = n - 1;
    while (a[k] < a[j]) k--;
    swap(a[k], a[j]);
    int l = j + 1;
    int r = n - 1;
    while (l < r) {
        swap(a[l], a[r]);
        l++;
        r--;
    }
    return true;
}

void print(vector<int> a) {
    //static int num = 1;
    cout.width(3);
    //cout << num++ << ": ";
    for (int i = 0; i < a.size(); i++)
        cout << a[i] << " ";
    cout << endl;
}

int calculate(vector<int> a, vector<vector<int>> matrix) {
    int way = matrix[a[a.size() - 1]][a[0]];
    for (int i = 0; i < a.size() - 1; i++) {
        way += matrix[a[i]][a[i + 1]];
    }
    return way;
}

int main() {
    string filename = "task1.txt";
    vector<vector<int>> matrix;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            vector<int> row;
            istringstream iss(line);
            string num;
            while (iss >> num) {
                if (num > "99") { row.push_back(INF); }
                else { row.push_back(stod(num)); }
            }matrix.push_back(row);
        }
        file.close();
        for (auto row : matrix) {
            for (auto num : row) { cout << num << " "; }
            cout << endl;
        }
    }
    else { cout << "Error opening file: " << filename << endl; }

    vector<int> a;
    for (int i = 0; i < matrix.size(); i++) {
        a.push_back(i);
    }
    int minway = calculate(a, matrix);
    vector<int> way = a;
    while (gen(a, matrix.size())) {
        if (minway > calculate(a, matrix)) {
            minway = calculate(a, matrix);
            way = a;
        }
    }
    cout << "Mit path is: ";
    for (int i = 0; i < way.size(); i++) {
        cout << way[i] + 1 << " ";
    }
    cout << " distance is: " << minway;
}