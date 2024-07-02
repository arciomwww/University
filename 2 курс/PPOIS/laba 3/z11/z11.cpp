#include <windows.h>
#include <string>
#include "Resource.h"

// Глобальные переменные
HWND g_hMainWindow;
int g_numStrings = 0;
std::wstring g_lastString;
// Прототипы функций
INT_PTR CALLBACK InputDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// Функция для создания диалогового окна ввода строки
void ShowInputDialog(HWND hwnd)
{
    DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_INPUT_WINDOW), hwnd, InputDlgProc);
}

// Обработчик сообщений диалогового окна ввода строки
INT_PTR CALLBACK InputDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        {
            wchar_t buffer[256];
            GetDlgItemText(hwnd, IDC_EDIT, buffer, 256); // Получаем текст из поля ввода

            // Преобразуем введенное число в целочисленное значение
            float number = std::wcstof(buffer, nullptr);

            // Возводим число в квадрат
            int squared = number * number;

            // Формируем сообщение с результатом
            std::wstring message = L"Квадрат числа " + std::to_wstring(number) + L" равен " + std::to_wstring(squared);

            MessageBox(hwnd, message.c_str(), L"Результат", MB_OK | MB_ICONINFORMATION);
        }
        EndDialog(hwnd, IDOK);
        return TRUE;

        case IDCANCEL:
            EndDialog(hwnd, IDCANCEL);
            return TRUE;
        }
        break;
    }

    return FALSE;
}

// Процедура окна
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        // Загружаем меню из ресурсов
        HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_Z11));
        SetMenu(hwnd, hMenu);
    }
    return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDM_ENTER:
            // Отображаем диалоговое окно для ввода строки
            ShowInputDialog(hwnd);
            return 0;

        case IDM_EXIT:
            // Закрываем окно
            DestroyWindow(hwnd);
            return 0;
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Отображаем текст о колич

        EndPaint(hwnd, &ps);
    }
    return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

// Главная функция
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Регистрация класса окна
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MainWindowClass";
    RegisterClassW(&wc);

    // Создание окна
    HWND hwnd = CreateWindowW(L"MainWindowClass", L"Главное окно", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 200, NULL, NULL, hInstance, NULL);

    // Отображение окна
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Цикл обработки сообщений
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
