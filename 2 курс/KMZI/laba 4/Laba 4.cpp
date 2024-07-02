#include <iostream>
#include <cmath>
#include <random>
#include <locale>

using namespace std;

// Функция для нахождения наибольшего общего делителя (НОД)
int gcd(int a, int b) {
    while (b) {
        int temp = a;
        a = b;
        b = temp % b;
    }
    return a;
}

// Функция для нахождения наибольшего простого делителя
int largest_prime_factor(int n) {
    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) {
            while (n % i == 0)
                n /= i;
            return i;
        }
    }
    return n;
}

// Функция для проверки простоты числа
bool is_prime(int n) {
    if (n <= 1)
        return false;
    for (int a = 2; a <= sqrt(n); a++) {
        int b = 2;
        while (pow(a, b) <= n) {
            if (pow(a, b) == n) {
                return false;
            }
            b++;
        }
    }
    int r = 2;
    while (r < n) {
        if (gcd(r, n) != 1)
            return false;

        if (is_prime(r)) {
            int q = largest_prime_factor(r - 1);
            if (q > 4 * sqrt(r) * log2(n) && static_cast<long long>(pow(n, (r - 1) / q)) % r != 1)
                return false;
        }

        r++;
    }

    if (r == n)
        return true;
    if (n <= 2 * sqrt(r) * log2(n)) {
        int a = n - 1;
        if (gcd(a, n) != 1)
            return false;
    }
    else {
        int a = static_cast<int>(1.25 * sqrt(r) * log2(n));
        if (static_cast<long long>(pow(a, n)) % n != (static_cast<long long>(pow(a, n - 1)) - a) % n)
            return false;
    }

    return true;
}


int generate_random_prime(int min_value, int max_value) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(min_value, max_value);

    int random_num;
    do {
        random_num = dis(gen);
    } while (!is_prime(random_num));

    return random_num;
}

int main() {
    setlocale(LC_ALL, "Russian");
    int random_prime = generate_random_prime(1, 100);
    cout << "Сгенерированное случайное простое число: " << random_prime << endl;

    return 0;
}
