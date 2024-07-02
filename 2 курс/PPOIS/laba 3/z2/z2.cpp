#include "framework.h"
#include "z2.h"

#define MAX_LOADSTRING 100
#include <windows.h>
bool infoDlgVisible = false;
// Глобальные переменные
HWND hMainDlg; // Дескриптор главного диалогового окна
HWND hInfoDlg; // Дескриптор диалогового окна с информацией

// Прототипы функций
LRESULT CALLBACK MainDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK InfoDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Функция WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Регистрация класса окна для диалогового окна информации
    WNDCLASSEX wcex_info;
    wcex_info.cbSize = sizeof(WNDCLASSEX);
    wcex_info.style = CS_HREDRAW | CS_VREDRAW;
    wcex_info.lpfnWndProc = InfoDlgProc;
    wcex_info.cbClsExtra = 0;
    wcex_info.cbWndExtra = 0;
    wcex_info.hInstance = hInstance;
    wcex_info.hIcon = NULL;
    wcex_info.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex_info.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex_info.lpszMenuName = NULL;
    wcex_info.lpszClassName = L"INFO_DIALOG";
    wcex_info.hIconSm = NULL;
    RegisterClassEx(&wcex_info);

    // Регистрация класса окна для главного диалогового окна
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = MainDlgProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"MainDialog";
    wcex.hIconSm = NULL;
    RegisterClassEx(&wcex);

    // Создание главного диалогового окна
    hMainDlg = CreateWindowEx(0, L"MainDialog", L"Главное окно", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, hInstance, NULL);

    // Отображение окна
    ShowWindow(hMainDlg, nCmdShow);
    UpdateWindow(hMainDlg);

    // Цикл обработки сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// Процедура окна
LRESULT CALLBACK MainDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        // Создание кнопки ОК
        CreateWindow(L"BUTTON", L"ОК", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            150, 100, 100, 50, hWnd, (HMENU)IDOK, NULL, NULL);
        return 0;

    case WM_COMMAND:
        // Обработка команд
        if (LOWORD(wParam) == IDOK)
        {
            // Если диалоговое окно не показано, показываем его
            if (!infoDlgVisible)
            {
                hInfoDlg = CreateWindowEx(0, L"INFO_DIALOG", L"Информация о событии", WS_OVERLAPPEDWINDOW,
                    CW_USEDEFAULT, CW_USEDEFAULT, 300, 200, NULL, NULL, GetModuleHandle(NULL), NULL);
                ShowWindow(hInfoDlg, SW_SHOWNORMAL);
                UpdateWindow(hInfoDlg);
                infoDlgVisible = true;
            }
            else // Иначе прячем его
            {
                ShowWindow(hInfoDlg, SW_HIDE);
                UpdateWindow(hInfoDlg);
                infoDlgVisible = false;
            }
        }
        return 0;


    case WM_CLOSE:
        // Закрытие приложения при нажатии на системную кнопку
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Процедура окна информации
LRESULT CALLBACK InfoDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CLOSE:
        // Закрытие диалогового окна информации
        DestroyWindow(hWnd);
        return 0;

    case WM_LBUTTONDOWN:
        MessageBox(hWnd, L"окно нажато", L"Информация", MB_OK);

    case WM_DESTROY:
        // Освобождение ресурсов и завершение работы диалогового окна информации
        
        return 0;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
