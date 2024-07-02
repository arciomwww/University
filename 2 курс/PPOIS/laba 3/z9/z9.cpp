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
    DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_INPUT_DIALOG), hwnd, InputDlgProc);
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
            GetDlgItemText(hwnd, IDC_EDIT_INPUT, buffer, 256); // Получаем текст из поля ввода
            
                g_lastString = buffer; // Сохраняем последнюю введенную строку
                MessageBox(hwnd, g_lastString.c_str(), L"Последняя введенная строка", MB_OK | MB_ICONINFORMATION);
            
            
            ++g_numStrings; // Увеличиваем количество введенных строк
            // Обновляем количество строк в главном окне
            wchar_t numStringsText[32];
            swprintf_s(numStringsText, L"Количество введенных строк: %d", g_numStrings);
            SetDlgItemText(g_hMainWindow, IDC_STATIC_NUMSTRINGS, numStringsText);
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
        // Создаем меню
        HMENU hMenu = CreateMenu();
        AppendMenu(hMenu, MF_STRING, IDM_MENU_INPUT, L"Ввод");
        AppendMenu(hMenu, MF_STRING, IDM_MENU_OUTPUT, L"Вывод");
        SetMenu(hwnd, hMenu);

        // Создаем статическое окно для отображения количества строк
        CreateWindowW(L"STATIC", L"Количество введенных строк: 0",
            WS_VISIBLE | WS_CHILD,
            10, 10, 250, 20,
            hwnd, (HMENU)IDC_STATIC_NUMSTRINGS, NULL, NULL);

        g_hMainWindow = hwnd; // Сохраняем дескриптор главного окна
    }
    return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDM_MENU_INPUT:
            ShowInputDialog(hwnd); // Отображаем диалоговое окно для ввода строки
            return 0;

        case IDM_MENU_OUTPUT:
        {
            wchar_t numStringsText[32];
            swprintf_s(numStringsText, L"Количество введенных строк: %d", g_numStrings);
            MessageBox(hwnd, numStringsText, L"Количество введенных строк: ", MB_OK | MB_ICONINFORMATION);
            MessageBox(hwnd, g_lastString.c_str(), L"Последняя введенная строка", MB_OK | MB_ICONINFORMATION);
        }
        return 0;
        }
        break;

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
