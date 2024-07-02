// lab2.3.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "lab2.3.h"

#define MAX_LOADSTRING 100
#include "framework.h"
#include "lab2.3.h"
#include <windows.h>

// Прототип функции обработки сообщений диалогового окна
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Создание диалогового окна
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG), NULL, DialogProc);

    return 0;
}

// Функция обработки сообщений диалогового окна
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_INITDIALOG: {
        // Инициализация элементов управления
        return TRUE;
    }
    case WM_COMMAND: {
        // Обработка нажатия кнопки "Ввести"
        if (LOWORD(wParam) == IDC_BUTTON_ENTER && HIWORD(wParam) == BN_CLICKED) {
            // Получение текста из поля ввода
            WCHAR buffer[256];
            GetDlgItemText(hwndDlg, IDC_EDIT_INPUT, buffer, sizeof(buffer) / sizeof(buffer[0]));

            // Вывод текста в поле вывода
            SetDlgItemText(hwndDlg, IDC_STATIC_OUTPUT, buffer);
        }
        return TRUE;
    }
    case WM_CLOSE:
        EndDialog(hwndDlg, 0);
        return TRUE;
    default:
        return FALSE;
    }
}
