#include <windows.h>
#include "Resource.h"
// Глобальные переменные
HWND g_hDlg;
HWND g_hListBox;
HWND g_hEdit;

// Прототипы функций
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

// Функция для создания диалогового окна
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG), NULL, DlgProc);
}

// Обработчик сообщений диалогового окна
INT_PTR CALLBACK DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
    {
        g_hDlg = hwnd;
        g_hListBox = GetDlgItem(hwnd, IDC_LISTBOX);
        g_hEdit = GetDlgItem(hwnd, IDC_EDIT2);
        // Инициализация списка строк z8
        const wchar_t* initialStrings[] = { L"Иванов", L"Петров", L"Сидоров" };
        const int numInitialStrings = sizeof(initialStrings) / sizeof(initialStrings[0]);

        for (int i = 0; i < numInitialStrings; ++i) {
            SendMessageW(g_hListBox, LB_ADDSTRING, 0, (LPARAM)initialStrings[i]);
        }

        
    }
    return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_CHANGE_LIST:
        {
            // Обработка нажатия кнопки "Изменить запись"
            int selectedIndex = SendMessage(g_hListBox, LB_GETCURSEL, 0, 0);
            if (selectedIndex != LB_ERR) {
                wchar_t buffer[256];
                GetWindowTextW(GetDlgItem(g_hDlg, IDC_EDIT2), buffer, 256); // Получаем текст из IDC_EDIT2

                if (wcslen(buffer) > 0) {
                    SendMessageW(g_hListBox, LB_DELETESTRING, selectedIndex, 0); // Удаляем текущую запись
                    SendMessageW(g_hListBox, LB_INSERTSTRING, selectedIndex, (LPARAM)buffer); // Вставляем новую запись на тоже место
                    SetWindowTextW(GetDlgItem(g_hDlg, IDC_EDIT2), L""); // Очищаем поле ввода
                }
            }
            return TRUE;
        }
        case IDC_BUTTON_ADD:
        {
            // Обработка нажатия кнопки "Добавить"
            wchar_t buffer[256];
            GetWindowTextW(GetDlgItem(g_hDlg, IDC_EDIT2), buffer, 256); // Получаем текст из IDC_EDIT2

            if (wcslen(buffer) > 0) {
                // Добавляем новую запись в список
                SendMessageW(g_hListBox, LB_ADDSTRING, 0, (LPARAM)buffer);
                SetWindowTextW(GetDlgItem(g_hDlg, IDC_EDIT2), L""); // Очищаем поле ввода
            }
        }
        return true;

        case IDC_BUTTON_DELETE:
        {
            // Обработка нажатия кнопки "Удалить"
            int index = SendMessage(g_hListBox, LB_GETCURSEL, 0, 0);
            if (index != LB_ERR)
                SendMessage(g_hListBox, LB_DELETESTRING, (WPARAM)index, 0);
        }
        return TRUE;

        case IDCANCEL:
            EndDialog(hwnd, 0);
            return TRUE;
        }
        
        
        break;

    case WM_CLOSE:
        EndDialog(hwnd, 0);
        return TRUE;
    }

    return FALSE;
}
