// lab2.1.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "lab2.1.h"

#define MAX_LOADSTRING 100
#include <windows.h>
#include <string>
#include <vector>

// Массив строк для вывода
std::vector<std::wstring> strings = { L"Use", L"function", L"prototype", L"Get", L"Text", L"ExtentPoint()" };

// Данные о длине строк и интервалах между ними
int lineHeight = 20; // Высота строки
int lineSpacing = 5; // Интервал между строками

// Функция отрисовки текста
void DrawText(HDC hdc) {
    int yPos = 0; // Начальная позиция по Y

    for (const auto& str : strings) {
        TextOut(hdc, 0, yPos, str.c_str(), str.length());
        yPos += lineHeight + lineSpacing;
    }
}

// Обработчик сообщений окна
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        DrawText(hdc);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

// Основная функция приложения
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Регистрация класса окна
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SampleWindowClass";

    RegisterClass(&wc);

    // Создание окна
    HWND hwnd = CreateWindowEx(
        0,
        L"SampleWindowClass",
        L"Array of Strings",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        return 0;
    }

    // Цикл обработки сообщений
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
