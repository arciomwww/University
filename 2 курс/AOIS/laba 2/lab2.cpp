#include <iostream>
#include <vector>
#include <cmath>
#include<locale>
#include <fstream>
#include <random>

class NeuralNetwork {
private:
    double alpha; // коэффициент обучения
    int neurons; // количество нейронов
    std::vector<double> W; // веса
    double T; // порог

public:
    NeuralNetwork(double alpha = 0.01, int neurons = 5) : alpha(alpha), neurons(neurons), T(0.0) {
        // Инициализация весов случайными значениями
        //предоставляет доступ к источнику случайных чисел в системе
        std::random_device rd;
        //генератор псевдослучайных чисел
        std::mt19937 gen(rd());
        //генерирует случайные вещественные числа
        std::uniform_real_distribution<> dis(0.0, 1.0);
        for (int i = 0; i < neurons; ++i) {
            W.push_back(dis(gen));
        }
    }

    // Создание набора данных
    std::vector<std::vector<double>> createDataset(const std::vector<double>& y) {
        std::vector<std::vector<double>> dataset;
        for (size_t i = neurons; i < y.size(); ++i) {
            std::vector<double> temp;
            for (int j = i - neurons; j < i; ++j) {
                temp.push_back(y[j]);
            }
            dataset.push_back(temp);
        }
        return dataset;
    }

    // Среднеквадратичная ошибка
    double MSE(const std::vector<double>& x, double e) {
        double prediction = predict(x);
        return 0.5 * pow((prediction - e), 2);
    }

    // Прогнозирование по матричному умножение
    double predict(const std::vector<double>& x) {
        double sum = 0.0;
        for (size_t i = 0; i < x.size(); ++i) {
            sum += W[i] * x[i];
        }
        return sum - T;
    }

    // Обучение
    void train(const std::vector<std::vector<double>>& X, const std::vector<double>& E) {
        for (size_t i = 0; i < X.size(); ++i) {
            double x = predict(X[i]);
            for (int j = 0; j < neurons; ++j) {
                W[j] -= alpha * (x - E[i]) * X[i][j];
            }
            T += alpha * (x - E[i]);
        }
    }

    // Тестирование
    double test(const std::vector<std::vector<double>>& X, const std::vector<double>& E) {
        double totalError = 0.0;
        for (size_t i = 0; i < X.size(); ++i) {
            totalError += MSE(X[i], E[i]);
        }
        return totalError / X.size();
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    double alpha = 0.01;
    int neurons = 5;

    std::ofstream out("output.txt");

    NeuralNetwork model(alpha, neurons);

    // Генерация набора данных
    std::vector<double> y;
    for (double x = 0.0; x < 5.0; x += 0.1) {
        double value = 3.0 * sin(7.0 * x) + 0.3;
        y.push_back(value);
    }
    std::vector<std::vector<double>> X = model.createDataset(y);

    // Обучение модели
    for (int epoch = 1; epoch <= 100; ++epoch) {
        model.train(X, y);
        double error = model.test(X, y);
        std::cout << "Эпоха: " << epoch << ", Ошибка: " << error << std::endl;
        out << epoch << " " << error << std::endl;
        if (error <= 1e-5) {
            std::cout << "Обучение завершено." << std::endl;
            break;
        }
    }

    // Тестирование обученной модели
    std::vector<double> predictions;
    for (size_t i = 0; i < y.size() - neurons; ++i) {
        predictions.push_back(model.predict(X[i]));
    }

    std::ofstream out1("graph.txt");

    double k = 0;
    for (auto num : predictions) {
        out1 << k <<" " << 3.0 * sin(7.0 * k) + 0.3 << " " << num << "\n";
        k += 0.1;
    }

    return 0;
}
