// lab2.2.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "lab2.2.h"

#define MAX_LOADSTRING 100
#include <windows.h>
#include <string>

// Прототип функции окна приложения
LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Функция создания и отображения диалогового окна
void ShowDialog(HWND hwndParent) {
    MessageBox(hwndParent, L"Диалоговое окно с кнопками", L"Диалог", MB_OKCANCEL);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Регистрация класса окна приложения
    WNDCLASS wc = {};
    wc.lpfnWndProc = MainWindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MainWindowClass";

    RegisterClass(&wc);

    // Создание главного окна приложения
    HWND hwndMain = CreateWindowEx(
        0,
        L"MainWindowClass",
        L"Главное окно",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL, NULL, hInstance, NULL);

    // Отображение диалогового окна
    ShowDialog(hwndMain);

    // Цикл обработки сообщений главного окна
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Обработчик сообщений главного окна
LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}
