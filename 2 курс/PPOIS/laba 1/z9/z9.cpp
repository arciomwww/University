// z9.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "z9.h"

#include <windows.h>
#include <stdio.h>

// Глобальные переменные
int clickCount = 0; // Счетчик нажатий левой кнопки мыши

// Прототипы функций
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawClickCount(HWND hwnd);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Регистрация класса окна
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"ClickCounter";

    RegisterClass(&wc);

    // Создание окна
    HWND hwnd = CreateWindowEx(
        0,                              // Расширенный стиль окна
        L"ClickCounter",                // Имя класса окна
        L"Click Counter App",           // Заголовок окна
        WS_OVERLAPPEDWINDOW,            // Стиль окна
        CW_USEDEFAULT, CW_USEDEFAULT,   // Позиция и размер окна
        400, 200,                       // Ширина и высота окна
        NULL,                           // Дескриптор родительского окна
        NULL,                           // Дескриптор меню окна
        hInstance,                      // Дескриптор экземпляра приложения
        NULL                            // Дополнительные параметры создания
    );

    // Отображение окна
    ShowWindow(hwnd, nCmdShow);

    // Цикл обработки сообщений
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_LBUTTONDOWN: // Обработка нажатия левой кнопки мыши
        clickCount++;
        break;
    case WM_RBUTTONDOWN: // Обработка нажатия правой кнопки мыши
        DrawClickCount(hwnd);
        InvalidateRect(hwnd, NULL, TRUE); // Инициировать перерисовку всего окна при нажатии правой кнопки мыши

        break;
    case WM_PAINT: // Обработка события перерисовки окна
        DrawClickCount(hwnd);
        break;
    case WM_DESTROY: // Обработка закрытия окна
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}


void DrawClickCount(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    // Форматирование текста для вывода счетчика нажатий
    WCHAR buffer[50];
    swprintf_s(buffer, L"Click Count: %d", clickCount);

    // Вывод текста на окно
    TextOut(hdc, 10, 10, buffer, wcslen(buffer));

    EndPaint(hwnd, &ps);
}
