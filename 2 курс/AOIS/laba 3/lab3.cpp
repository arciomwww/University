#include <iostream>
#include <vector>
#include <cmath>
#include <locale>

class Bidirectional {
private:
    std::vector<std::vector<int>> x;
    std::vector<std::vector<int>> y;
    std::vector<std::vector<int>> w;
    int n, m;

public:
    Bidirectional(std::vector<std::vector<int>> vectors, int n, int m) : n(n), m(m) {
        x.resize(vectors.size());
        y.resize(vectors.size());
        w.resize(n, std::vector<int>(m, 0));
        //инициализируем х и у
        for (size_t i = 0; i < vectors.size(); ++i) {
            x[i].resize(n);
            y[i].resize(m);

            for (int j = 0; j < n; ++j) {
                x[i][j] = vectors[i][j] <= 0 ? -1 : 1;
            }
            //Преобразование отрицательных в положительные
            for (int j = 0; j < m; ++j) {
                y[i][j] = vectors[i][vectors[0].size() - m + j] <= 0 ? -1 : 1;
            }
            //Вычисление матрицы весов
            for (int j = 0; j < n; ++j) {
                for (int k = 0; k < m; ++k) {
                    w[j][k] += x[i][j] * y[i][k];
                }
            }
        }
    }

    std::vector<int> nextY(const std::vector<int>& inputX) {
        std::vector<int> ans(m, 0);
        for (int i = 0; i < m; ++i) {
            int sum = 0;
            for (int j = 0; j < n; ++j) {
                sum += inputX[j] * w[j][i];
            }
            ans[i] = std::max(std::min(sum, 1), -1);
        }
        return ans;
    }

    std::vector<int> nextX(const std::vector<int>& inputY) {
        std::vector<int> ans(n, 0);
        for (int i = 0; i < n; ++i) {
            int sum = 0;
            for (int j = 0; j < m; ++j) {
                sum += inputY[j] * w[i][j];
            }
            ans[i] = std::max(std::min(sum, 1), -1);
        }
        return ans;
    }
    
    std::vector<int> calc(const std::vector<int>& inputVector) {
        std::vector<int> prevX, prevY;
        if (inputVector.size() == n) {
            prevX = inputVector;
            prevX = applyThreshold(prevX);
            prevY = nextY(prevX);
            for (int i = 0; i < 100; ++i) {
                std::vector<int> y = nextY(prevX);
                std::vector<int> x = nextX(y);
                if (y == prevY && x == prevX) {
                    return applyThreshold(y);
                }
                
                prevX = x;
                prevY = y;
            }
        }
        else {
            prevY = inputVector;
            prevY = applyThreshold(prevY);
            prevX = nextX(prevY);
            for (int i = 0; i < 100; ++i) {
                std::vector<int> x = nextX(prevY);
                std::vector<int> y = nextY(x);
                if (x == prevX && y == prevY) {
                    return applyThreshold(x);
                }
               
                prevX = x;
                prevY = y;
            }
        }
        return std::vector<int>();
    }



private:
    
    std::vector<int> applyThreshold(const std::vector<int>& input) {
        std::vector<int> result;
        for (int val : input) {
            result.push_back(val <= 0 ? 0 : val);
        }
        return result;
    }
};

std::vector<int> testModel(Bidirectional& model, std::vector<int> inputVector, int numBitsChanged) {
    std::vector<int> inputVectorCopy = inputVector;

    for (int i = 0; i < numBitsChanged; ++i) {
        int index = rand() % inputVectorCopy.size();
        inputVectorCopy[index] = 1 - inputVectorCopy[index];
    }

    return model.calc(inputVectorCopy);
}
//подсчитывает количество правильных прогнозов для каждого числа измененных битов.
std::vector<int> numPredicts(Bidirectional& model, std::vector<int> inputVector, int maxBitsToChange) {
    std::vector<int> numCorrectPredictions;
    for (int numBitsToChange = 1; numBitsToChange <= maxBitsToChange; ++numBitsToChange) {
        std::vector<int> predictedResult = testModel(model, inputVector, numBitsToChange);
        std::vector<int> expectedResult = model.calc(inputVector);
        int numCorrect = 0;
        for (size_t i = 0; i < predictedResult.size(); ++i) {
            if (predictedResult[i] == expectedResult[i]) {
                numCorrect++;
            }
        }
        numCorrectPredictions.push_back(numCorrect);
    }
    return numCorrectPredictions;
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::vector<int> Y1 = { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 };
    std::vector<int> Y2 = { 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1 };
    std::vector<int> Y3 = { 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1 };

    int inputSize;
    std::cout << "Введите размер ввода (8 или 12): ";
    std::cin >> inputSize;

    if (inputSize == 8 || inputSize == 12) {
        Bidirectional model({ Y1, Y2, Y3 }, 12, 8);

        std::vector<int> inputVector;
        int value;
        std::cout << "Введите " << inputSize << " значений через пробел: ";
        for (int i = 0; i < inputSize; ++i) {
            std::cin >> value;
            inputVector.push_back(value);
        }

        std::vector<int> result = model.calc(inputVector);
        for (int val : result) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "Неверный размер ввода. Пожалуйста, введите 8 или 12." << std::endl;
    }

    return 0;
}