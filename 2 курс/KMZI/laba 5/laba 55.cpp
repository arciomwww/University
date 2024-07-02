#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>

// Функция для вычисления наибольшего общего делителя
int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

// Структура для представления точек на кривой
struct Point {
    long x, y;
    bool infinity;

    Point() : x(0), y(0), infinity(true) {}
    Point(long x, long y) : x(x), y(y), infinity(false) {}
};

// Функция для сложения точек на эллиптической кривой
Point addPoints(const Point& P, const Point& Q, long a, long n) {
    if (P.infinity) return Q;
    if (Q.infinity) return P;

    long lambda;
    if (P.x == Q.x && P.y == Q.y) {
        lambda = (3 * P.x * P.x + a) * gcd(2 * P.y, n) % n;
    }
    else {
        lambda = (Q.y - P.y) * gcd(Q.x - P.x, n) % n;
    }

    long x3 = (lambda * lambda - P.x - Q.x) % n;
    long y3 = (lambda * (P.x - x3) - P.y) % n;

    if (x3 < 0) x3 += n;
    if (y3 < 0) y3 += n;

    return Point(x3, y3);
}

// Функция для факторизации числа методом Ленстры
void lenstraFactorization(long number) {
    long x1 = rand() % number;
    long y1 = rand() % number;
    long a = rand() % number;
    long b = (y1 * y1 - x1 * x1 * x1 - a * x1) % number;

    if (b < 0) b += number;

    Point P(x1, y1);

    for (long k = 1; k < number; ++k) {
        Point Q = P;
        for (long j = 1; j < k; ++j) {
            Q = addPoints(Q, P, a, number);
            long d = gcd(Q.y, number);
            if (d > 1 && d < number) {
                std::cout << d << " ";
                number /= d;
            }
        }
    }

    if (number > 1) {
        std::cout << number << " ";
    }
}

// Функция для факторизации числа методом пробных делений
void trialDivisionFactorization(long number) {
    while (number % 2 == 0) {
        std::cout << "2 ";
        number /= 2;
    }

    for (int i = 3; i <= std::sqrt(number); i += 2) {
        while (number % i == 0) {
            std::cout << i << " ";
            number /= i;
        }
    }

    if (number > 2) {
        std::cout << number << " ";
    }
}

// Функция для комбинированной факторизации числа
void combinedFactorization(long number) {
    auto start = std::chrono::high_resolution_clock::now();

    // Проверка на простое число
    bool isPrime = true;
    for (int i = 2; i <= std::sqrt(number); ++i) {
        if (number % i == 0) {
            isPrime = false;
            break;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time to check for a prime number: " << duration.count() << " microseconds." << std::endl;

    if (isPrime) {
        std::cout << "The number is prime." << std::endl;
        return;
    }

    std::cout << "Factorization result: ";

    start = std::chrono::high_resolution_clock::now();

    // Попытка факторизации методом пробных делений
    trialDivisionFactorization(number);

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << std::endl << "Trial division method time: " << duration.count() << " microseconds." << std::endl;

    // Если метод пробных делений не дал результат, пробуем метод Ленстры
    if (number > 1) {
        std::cout << "Result of factorization by Lenstra method: ";

        start = std::chrono::high_resolution_clock::now();

        lenstraFactorization(number);

        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << std::endl << "Lenstra method time: " << duration.count() << " microseconds." << std::endl;
    }
}

int main() {
    long number;
    int method;

    std::cout << "Enter the number to factorize (or enter 0 to exit): ";
    std::cin >> number;

    while (true) {
        std::cout << std::endl << "Select factorization method:" << std::endl;
        std::cout << "1. Trial division method" << std::endl;
        std::cout << "2. Lenstra method" << std::endl;
        std::cout << "3. Combined method" << std::endl;
        std::cout << "4. Enter a new number" << std::endl;
        std::cout << "5. Exit" << std::endl;
        std::cout << "Your choice: ";
        std::cin >> method;

        std::cout << "Factorization result: ";

        auto start = std::chrono::high_resolution_clock::now();

        switch (method) {
        case 1:
            trialDivisionFactorization(number);
            break;
        case 2:
            lenstraFactorization(number);
            break;
        case 3:
            combinedFactorization(number);
            break;
        case 4:
            std::cout << "Enter the new number: ";
            std::cin >> number;
            break;
        case 5:
            std::cout << "Exiting the program." << std::endl;
            return 0;
        default:
            std::cout << "Incorrect choice of method." << std::endl;
            continue;
        }
    }

    return 0;
}
