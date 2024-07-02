// z11.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "z11.h"

#define MAX_LOADSTRING 100
#include <windows.h>
#include <string>

// Глобальные переменные
int mouseX = 0; // Координата X курсора мыши
int mouseY = 0; // Координата Y курсора мыши

// Функция отрисовки текста с координатами
void DrawCoordinates(HWND hwnd, HDC hdc) {
    std::wstring text = L"X = " + std::to_wstring(mouseX) + L", Y = " + std::to_wstring(mouseY);
    TextOut(hdc, 10, 10, text.c_str(), text.size());
}

// Процедура окна
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rect;
        GetClientRect(hwnd, &rect);
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

        DrawCoordinates(hwnd, hdc);

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_RBUTTONDOWN: // При перемещении мыши
        // Получаем текущие координаты курсора мыши
        POINT cursorPos;
        GetCursorPos(&cursorPos);
        ScreenToClient(hwnd, &cursorPos); // Переводим в координаты окна
        mouseX = cursorPos.x;
        mouseY = cursorPos.y;
        // Инициируем перерисовку окна
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    case WM_DESTROY: // При закрытии окна
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Точка входа в программу
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Регистрация класса окна
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MouseCoordinatesWindow";

    RegisterClass(&wc);

    // Создание окна
    HWND hwnd = CreateWindowEx(
        0,                              // Расширенный стиль окна
        L"MouseCoordinatesWindow",      // Имя класса окна
        L"Mouse Coordinates Application", // Заголовок окна
        WS_OVERLAPPEDWINDOW,            // Стиль окна
        CW_USEDEFAULT, CW_USEDEFAULT,   // Позиция окна
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