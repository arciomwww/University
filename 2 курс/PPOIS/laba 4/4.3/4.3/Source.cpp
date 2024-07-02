#include <windows.h>
#include <vector>

// Глобальные переменные
std::vector<POINT> points; // Вектор для хранения координат точек

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

    hWnd = CreateWindow(L"myWindowClass", L"Drawing Application", WS_OVERLAPPEDWINDOW,
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
    HDC hdc;
    PAINTSTRUCT ps;

    switch (message)
    {
    case WM_LBUTTONDOWN: // Обработка нажатия левой клавиши мыши
        POINT point;
        point.x = LOWORD(lParam);
        point.y = HIWORD(lParam);
        points.push_back(point); // Сохранение координат точки в векторе
        InvalidateRect(hWnd, NULL, TRUE); // Вызов перерисовки окна
        break;
    case WM_RBUTTONDOWN: // Обработка нажатия правой клавиши мыши
        // Здесь можно добавить код для вывода изображения с использованием координат из вектора points
        break;
    case WM_PAINT: // Обработка сообщения о перерисовке окна
        hdc = BeginPaint(hWnd, &ps);
        // Здесь можно добавить код для рисования точек и соединения их прямыми линиями
        for (size_t i = 0; i < points.size(); ++i)
        {
            Ellipse(hdc, points[i].x - 3, points[i].y - 3, points[i].x + 3, points[i].y + 3); // Рисование точек
            if (i > 0)
            {
                MoveToEx(hdc, points[i - 1].x, points[i - 1].y, NULL);
                LineTo(hdc, points[i].x, points[i].y); // Соединение точек линиями
            }
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
