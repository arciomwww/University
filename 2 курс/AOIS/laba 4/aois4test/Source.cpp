#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <utility>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <omp.h>
#include <random>
#include <chrono>

//***************************************************************************************************************************************************
void load_data(const std::string& file_path, std::vector<std::vector<double>>& training_data, std::vector<std::vector<double>>& labels) {
    std::ifstream file(file_path);
    if (!file) {
        std::cerr << "Ошибка открытия файла!" << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Пропускаем первую строку с общим количеством изображений

    int image_count = 0;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int label;
        iss >> label;

        if (label < 0 || label > 9) {
            std::cerr << "Недопустимая метка класса: " << label << std::endl;
            continue;
        }

        std::vector<double> image_data;
        std::vector<double> one_hot_label(10, 0.0);
        one_hot_label[label] = 1.0;

        for (int i = 0; i < 28; i++) {
            std::getline(file, line);
            std::istringstream row(line);
            double value;
            int pixel_count = 0;
            while (row >> value) {
                image_data.push_back(value);
                pixel_count++;
            }
            if (pixel_count != 28) {
                std::cerr << "Неверное количество пикселей в строке " << i << ": " << pixel_count << std::endl;
                image_data.clear();
                break;
            }
        }

        if (image_data.size() == 28 * 28) {
            training_data.push_back(image_data);
            labels.push_back(one_hot_label);
            image_count++;
            if (image_count % 10000 == 0) {
                std::cout << "Загружено изображений: " << image_count << std::endl;
            }
        }
        else {
            std::cerr << "Неверный размер данных изображения: " << image_data.size() << std::endl;
        }
    }

    file.close();
    std::cout << "SIZE: " << training_data.size() << std::endl;
}
//***************************************************************************************************************************************************
class NeuralNetwork {
private:
    std::vector<std::vector<double>> weights_hidden; // веса скрытого слоя
    std::vector<std::vector<double>> weights_output; // веса выходного слоя
    std::vector<double> biases_hidden; // смещения скрытого слоя
    std::vector<double> biases_output; // смещения выходного слоя

public:
    NeuralNetwork(int input_size, int hidden_size, int output_size) {
        // Инициализация весов и смещений случайными числами от 0 до 1

        std::random_device rd;
        std::mt19937 generator(rd());

        // Инициализация весов скрытого слоя
        for (int i = 0; i < hidden_size; i++) {
            biases_hidden.push_back(0.0); // Инициализация смещений нулями
            std::vector<double> hidden_weights;
            for (int j = 0; j < input_size; j++) {
                double xavier_value = std::sqrt(6.0 / (input_size + hidden_size));
                std::uniform_real_distribution<double> distribution(-xavier_value, xavier_value);
                hidden_weights.push_back(distribution(generator));
            }
            weights_hidden.push_back(hidden_weights);
        }

        // Инициализация весов выходного слоя
        for (int i = 0; i < output_size; i++) {
            biases_output.push_back(0.0); // Инициализация смещений нулями
            std::vector<double> output_weights;
            for (int j = 0; j < hidden_size; j++) {
                double xavier_value = std::sqrt(6.0 / (hidden_size + output_size));
                std::uniform_real_distribution<double> distribution(-xavier_value, xavier_value);
                output_weights.push_back(distribution(generator));
            }
            weights_output.push_back(output_weights);
        }
    }
    void set_weights_hidden(const std::vector<std::vector<double>>& weights) {
        weights_hidden = weights;
    }

    void set_weights_output(const std::vector<std::vector<double>>& weights) {
        weights_output = weights;
    }

    void set_biases_hidden(const std::vector<double>& biases) {
        biases_hidden = biases;
    }

    void set_biases_output(const std::vector<double>& biases) {
        biases_output = biases;
    }

    const std::vector<std::vector<double>>& get_weights_hidden() const {
        return weights_hidden;
    }
    const std::vector<std::vector<double>>& get_weights_output() const {
        return weights_output;
    }
    const std::vector<double>& get_biases_hidden() const {
        return biases_hidden;
    }

    const std::vector<double>& get_biases_output() const {
        return biases_output;
    }

    double sigmoid(double x) {
        return 1 / (1 + exp(-x));
    }

    // Производная функции сигмоиды
    double sigmoid_derivative(double x) {
        double sig = sigmoid(x);
        return sig * (1 - sig);
    }
    std::vector<double> forward_propagation(const std::vector<double>& input) {
        std::vector<double> hidden_layer_output(biases_hidden.size());
        for (int j = 0; j < biases_hidden.size(); j++) {
            double hidden_output = biases_hidden[j];
            for (int k = 0; k < input.size(); k++) {
                hidden_output += weights_hidden[j][k] * input[k];
            }
            hidden_layer_output[j] = sigmoid(hidden_output);
        }

        std::vector<double> output_layer_output(biases_output.size());
        for (int j = 0; j < biases_output.size(); j++) {
            double output_neuron = biases_output[j];
            for (int k = 0; k < hidden_layer_output.size(); k++) {
                output_neuron += weights_output[j][k] * hidden_layer_output[k];
            }
            output_layer_output[j] = sigmoid(output_neuron);
        }

        return output_layer_output;
    }

    void backward_propagation(const std::vector<double>& output, const std::vector<double>& labels, const std::vector<double>& input, double learning_rate) {
        std::vector<double> hidden_layer_output(biases_hidden.size());
        for (int j = 0; j < biases_hidden.size(); j++) {
            double hidden_output = biases_hidden[j];
            for (int k = 0; k < input.size(); k++) {
                hidden_output += weights_hidden[j][k] * input[k];
            }
            hidden_layer_output[j] = sigmoid(hidden_output);
        }

        // Вычисление ошибки выходного слоя
        std::vector<double> output_error(biases_output.size());
        for (int j = 0; j < biases_output.size(); j++) {
            output_error[j] = (output[j] - labels[j]) * sigmoid_derivative(output[j]);
        }

        // Обновление весов и смещений выходного слоя
        for (int j = 0; j < biases_output.size(); j++) {
            biases_output[j] -= learning_rate * output_error[j];
            for (int k = 0; k < weights_output[j].size(); k++) {
                weights_output[j][k] -= learning_rate * output_error[j] * hidden_layer_output[k];
            }
        }

        // Вычисление ошибки скрытого слоя
        std::vector<double> hidden_error(biases_hidden.size());
        for (int j = 0; j < biases_hidden.size(); j++) {
            double weighted_output_error = 0.0;
            for (int k = 0; k < biases_output.size(); k++) {
                weighted_output_error += output_error[k] * weights_output[k][j];
            }
            hidden_error[j] = weighted_output_error * sigmoid_derivative(hidden_layer_output[j]);
        }

        // Обновление весов и смещений скрытого слоя
        for (int j = 0; j < biases_hidden.size(); j++) {
            biases_hidden[j] -= learning_rate * hidden_error[j];
            for (int k = 0; k < weights_hidden[j].size(); k++) {
                weights_hidden[j][k] -= learning_rate * hidden_error[j] * input[k];
            }
        }
    }


    int get_predicted_label(const std::vector<double>& output) {
        // Ищем индекс нейрона с наибольшим значением на выходном слое
        int max_index = 0;
        double max_value = output[0];
        for (int i = 1; i < output.size(); ++i) {
            if (output[i] > max_value) {
                max_value = output[i];
                max_index = i;
            }
        }
        return max_index; // Возвращаем индекс нейрона с наибольшим значением
    }

    int get_true_label(const std::vector<double>& labels) {
        // Ищем индекс ненулевого элемента в векторе меток
        for (int i = 0; i < labels.size(); ++i) {
            if (labels[i] != 0.0) {
                return i; // Возвращаем индекс ненулевого элемента, предполагая, что метка закодирована в виде вектора
            }
        }
        return -1; // Возвращаем -1, если не найдено ненулевых элементов (ошибка)
    }

    double calculate_loss(const std::vector<double>& output, const std::vector<double>& labels) {
        double loss = 0.0;

        for (int i = 0; i < output.size(); ++i) {
            double diff = output[i] - labels[i];
            loss += diff * diff;
        }

        loss = loss / output.size();

        return loss;
    }

    void train(const std::vector<std::vector<double>>& training_data, const std::vector<std::vector<double>>& labels, double learning_rate, int epochs) {
        // Инициализация весов и смещений
        for (int epoch = 0; epoch < epochs; epoch++) {
            double total_loss = 0.0;
            int correct_predictions = 0;

            // Замер времени начала эпохи
            auto start_time = std::chrono::steady_clock::now();

            for (size_t i = 0; i < training_data.size(); i++) {
                // Прямой проход (forward propagation)
                std::vector<double> output = forward_propagation(training_data[i]);

                // Расчет ошибки и общей потери
                double loss = calculate_loss(output, labels[i]);
                total_loss += loss;

                // Подсчет правильных предсказаний
                if (get_predicted_label(output) == get_true_label(labels[i])) {
                    correct_predictions++;
                }

                // Обратное распространение ошибки (backward propagation) и обновление весов
                backward_propagation(output, labels[i], training_data[i], learning_rate);
            }

            // Замер времени окончания эпохи
            auto end_time = std::chrono::steady_clock::now();


            double accuracy = (static_cast<double>(correct_predictions) / training_data.size()) * 100;

            // Вычисление времени, затраченного на эпоху в секундах
            std::chrono::duration<double> elapsed_seconds = end_time - start_time;

            std::cout << "Epoch: " << epoch + 1 << ", Loss: " << total_loss / training_data.size() << ", Accuracy: " << accuracy << "%, Time: " << elapsed_seconds.count() << " seconds" << std::endl;

            // Условия остановки обучения
            if (total_loss / training_data.size() <= 0.015 || accuracy >= 99.4) {
                std::cout << "Достигнуто условие остановки обучения." << std::endl;
                break;
            }
        }
    }
    void test(const std::vector<std::vector<double>>& test_data, const std::vector<std::vector<double>>& test_labels) {
        int correct_predictions = 0;
        int total_predictions = test_data.size();
        double total_loss = 0.0;
        for (size_t i = 0; i < test_data.size(); i++) {
            // Прямой проход (forward propagation)
            std::vector<double> output = forward_propagation(test_data[i]);
            double loss = calculate_loss(output, test_labels[i]);
            total_loss += loss;
            // Получение предсказанной метки и истинной метки
            int predicted_label = get_predicted_label(output);
            int true_label = get_true_label(test_labels[i]);

            // Подсчет правильных предсказаний
            if (predicted_label == true_label) {
                correct_predictions++;
            }
        }

        double accuracy = (static_cast<double>(correct_predictions) / total_predictions) * 100;

        std::cout << "Test Accuracy: " << accuracy << "%" << std::endl;
        std::cout << "Loss: " << total_loss / test_data.size() << std::endl;
    }

};


int main() {
    setlocale(LC_ALL, "RU");
    std::vector<std::vector<double>> training_data;
    std::vector<std::vector<double>> training_labels;
    std::vector<std::vector<double>> testing_data;
    std::vector<std::vector<double>> testing_labels;

    int choice;
    std::cout << "1. Обучить модель заново" << std::endl;
    std::cout << "2. Тестирование на основе сохраненных весов и смещений" << std::endl;
    std::cout << "Выберите опцию: ";
    std::cin >> choice;

    if (choice == 1) {
        // Загрузка данных MNIST
        load_data("lib_MNIST_edit.txt", training_data, training_labels);
        std::cout << "dataset loaded." << std::endl;

        NeuralNetwork nn(784, 800, 10);
        nn.train(training_data, training_labels, 0.01, 20);

        // Сохранение весов в файл после обучения
        std::ofstream weights_file("final_weights.txt");
        if (weights_file.is_open()) {
            // Запись весов в файл
            for (const auto& layer_weights : { nn.get_weights_hidden(), nn.get_weights_output() }) {
                for (const auto& neuron_weights : layer_weights) {
                    for (const auto& weight : neuron_weights) {
                        weights_file << weight << " ";
                    }
                    weights_file << "\n";
                }
            }
            for (const auto& bias : nn.get_biases_hidden()) {
                weights_file << bias << " ";
            }
            weights_file << "\n";
            for (const auto& bias : nn.get_biases_output()) {
                weights_file << bias << " ";
            }
            weights_file << "\n";
            weights_file.close();
            std::cout << "Final weights saved to final_weights.txt" << std::endl;
        }
        else {
            std::cerr << "Unable to open file for writing final weights." << std::endl;
        }

        load_data("lib_10k.txt", testing_data, testing_labels);
        std::cout << "testdata loaded." << std::endl;
        nn.test(testing_data, testing_labels);
    }
    // Загрузка весов и смещений из файла
    std::ifstream weights_file("final_weights.txt");
    if (weights_file.is_open()) {
        // Чтение весов из файла
        std::vector<std::vector<double>> hidden_weights(800, std::vector<double>(784));
        std::vector<std::vector<double>> output_weights(10, std::vector<double>(800));
        std::vector<double> hidden_biases(800), output_biases(10);

        // Чтение весов скрытого слоя
        for (int i = 0; i < 800; ++i) {
            std::string line;
            std::getline(weights_file, line);
            std::stringstream ss(line);
            int count = 0;
            for (int j = 0; j < 784; ++j) {
                if (ss >> hidden_weights[i][j]) {
                    count++;
                }
                else {
                    std::cerr << "Error reading hidden weights at row " << i << ", column " << j << std::endl;
                    break;
                }
            }
            if (count != 784) {
                std::cerr << "Error: Expected 784 hidden weights in row " << i << ", but found " << count << std::endl;
            }
        }

        // Чтение весов выходного слоя
        for (int i = 0; i < 10; ++i) {
            std::string line;
            std::getline(weights_file, line);
            std::stringstream ss(line);
            int count = 0;
            for (int j = 0; j < 800; ++j) {
                if (ss >> output_weights[i][j]) {
                    count++;
                }
                else {
                    std::cerr << "Error reading output weights at row " << i << ", column " << j << std::endl;
                    break;
                }
            }
            if (count != 300) {
                std::cerr << "Error: Expected 300 output weights in row " << i << ", but found " << count << std::endl;
            }
        }

        // Чтение смещений скрытого слоя
        std::string line;
        std::getline(weights_file, line);
        std::stringstream ss(line);
        int count = 0;
        for (int i = 0; i < 800; ++i) {
            if (ss >> hidden_biases[i]) {
                count++;
            }
            else {
                std::cerr << "Error reading hidden bias at index " << i << std::endl;
                break;
            }
        }
        if (count != 300) {
            std::cerr << "Error: Expected 300 hidden biases, but found " << count << std::endl;
        }

        // Чтение смещений выходного слоя
        std::getline(weights_file, line);
        ss = std::stringstream(line);
        count = 0;
        for (int i = 0; i < 10; ++i) {
            if (ss >> output_biases[i]) {
                count++;
            }
            else {
                std::cerr << "Error reading output bias at index " << i << std::endl;
                break;
            }
        }
        if (count != 10) {
            std::cerr << "Error: Expected 10 output biases, but found " << count << std::endl;
        }

        weights_file.close();

        if (hidden_weights.size() == 800 && output_weights.size() == 10 &&
            hidden_biases.size() == 800 && output_biases.size() == 10) {
            NeuralNetwork nn(784, 800, 10);
            nn.set_weights_hidden(hidden_weights);
            nn.set_weights_output(output_weights);
            nn.set_biases_hidden(hidden_biases);
            nn.set_biases_output(output_biases);

            load_data("lib_10k.txt", testing_data, testing_labels);
            std::cout << "testdata loaded." << std::endl;
            nn.test(testing_data, testing_labels);
        }
        else {
            std::cerr << "Error: Invalid weights or biases loaded from file." << std::endl;
        }
    }
    else {
        std::cerr << "Unable to open file for reading weights." << std::endl;
    }
}