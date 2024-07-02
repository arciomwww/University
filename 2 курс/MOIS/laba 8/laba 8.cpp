#include <iostream>
#include <vector>
#include <locale>
#include <time.h>
#include <fstream>

using namespace std;

bool isSafe(vector<vector<int>>& board, int row, int col, int n) {
    static const int dx[] = { 2, 1, -1, -2, -2, -1, 1, 2 };
    static const int dy[] = { 1, 2, 2, 1, -1, -2, -2, -1 };

    // Проверка хода коня
    for (int i = 0; i < 8; ++i) {
        int x = row + dx[i];
        int y = col + dy[i];
        if (x >= 0 && x < n && y >= 0 && y < n && board[x][y] == 1) {
            return false;
        }
    }
    // Проверка вертикали сверху вниз
    for (int i = 0; i < row; ++i) {
        if (board[i][col] == 1) {
            return false;
        }
    }

    // Проверка левой диагонали сверху вниз
    for (int i = row, j = col; i >= 0 && j >= 0; --i, --j) {
        if (board[i][j] == 1) {
            return false;
        }
    }

    // Проверка правой диагонали сверху вниз
    for (int i = row, j = col; i >= 0 && j < n; --i, ++j) {
        if (board[i][j] == 1) {
            return false;
        }
    }

    return true;
}

bool solveMagarajaUtil(vector<vector<int>>& board, int row, int n) {
    if (row == n) {
        return true;
    }

    for (int col = 0; col < n; ++col) {
        if (isSafe(board, row, col, n)) {
            board[row][col] = 1;
            if (solveMagarajaUtil(board, row + 1, n)) {
                return true;
            }
            board[row][col] = 0;
        }
    }

    return false;
}

bool solveMagaraja(int n) {
    vector<vector<int>> board(n, vector<int>(n, 0));

    if (!solveMagarajaUtil(board, 0, n)) {
        cout << "Решение не найдено" << endl;
        return false;
    }

    cout << "Расстановка магараджей:" << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }

    return true;
}

// Пример использования
int main() {
    setlocale(LC_ALL, "Russian");
    ofstream out("dots.txt");
    int n = 1; // Размер доски
    double seconds = 0;
    while (seconds < 150) {
        clock_t start = clock();
        solveMagaraja(n);
        clock_t end = clock();
        seconds = (double)(end - start) / CLOCKS_PER_SEC;
        printf("The time: %f seconds\n", seconds);
        out << n << " " << seconds << endl;
        n += 1;
    }
    
    return 0;
}