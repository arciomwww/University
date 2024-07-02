#include <windows.h>
#include <wchar.h>

// Глобальные переменные
wchar_t g_char = L' '; // Символ для ввода
POINT g_mousePos = { 0 }; // Координаты для вывода

// Прототипы функций
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Точка входа для приложения
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HWND hWnd;
    MSG lpMsg;
    WNDCLASS wcApp;
    wcApp.style = CS_HREDRAW | CS_VREDRAW;
    wcApp.lpfnWndProc = WndProc;
    wcApp.cbClsExtra = 0;
    wcApp.cbWndExtra = 0;
    wcApp.hInstance = hInstance;
    wcApp.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcApp.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcApp.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcApp.lpszMenuName = NULL;
    wcApp.lpszClassName = L"myWindowClass";

    if (!RegisterClass(&wcApp))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hWnd = CreateWindow(L"myWindowClass", L"Keyboard Input and Echo", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400, NULL, NULL, hInstance, NULL);

    if (hWnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&lpMsg, NULL, 0, 0))
    {
        TranslateMessage(&lpMsg);
        DispatchMessage(&lpMsg);
    }

    return lpMsg.wParam;
}

// Обработчик сообщений для главного окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_CHAR: // Обработка ввода символа с клавиатуры
        g_char = (wchar_t)wParam;
        InvalidateRect(hWnd, NULL, TRUE); // Вызов перерисовки окна
        break;
    case WM_LBUTTONDOWN: // Обработка нажатия левой клавиши мыши
        g_mousePos.x = LOWORD(lParam);
        g_mousePos.y = HIWORD(lParam);
        InvalidateRect(hWnd, NULL, TRUE); // Вызов перерисовки окна
        break;
    case WM_PAINT: // Обработка сообщения о перерисовке окна
        hdc = BeginPaint(hWnd, &ps);
        // Вывод символа или его эхо-вывод
        if (g_mousePos.x == 0 && g_mousePos.y == 0)
        {
            TextOut(hdc, 10, 10, &g_char, 1); // Вывод в левый верхний угол
        }
        else
        {
            TextOut(hdc, g_mousePos.x, g_mousePos.y, &g_char, 1); // Вывод в позицию мыши
        }
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY: // Обработка сообщения об уничтожении окна
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
