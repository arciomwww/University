#include <windows.h>
#include <string>

// Глобальные переменные
int posX = 0; // Начальная позиция по X
int posY = 0; // Начальная позиция по Y
int width = 400;
int height = 200;
// Функция отрисовки текста
void DrawText(HWND hwnd, HDC hdc) {
    std::wstring text = L"Работает ТКП";
    TextOut(hdc, posX, posY, text.c_str(), text.size());
}

// Процедура окна
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Заполняем окно фоновым цветом, чтобы стереть предыдущий текст
        RECT rect;
        GetClientRect(hwnd, &rect);
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

        DrawText(hwnd, hdc);
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_LBUTTONDOWN: // При нажатии левой кнопки мыши
        if (posX < height) {
            posX += 50; // Уменьшаем координату X на 50
            posY += 50;
        }
        else {
            posX = 0; // Уменьшаем координату X на 50
            posY = 0;
        }
        InvalidateRect(hwnd, NULL, TRUE); // Инициируем перерисовку окна
        break;
    case WM_RBUTTONDOWN: // При нажатии правой кнопки мыши
        if (posX > 0) {
            posX -= 50; // Уменьшаем координату X на 50
            posY -= 50;
        }
        else {
            posX = height; // Уменьшаем координату X на 50
            posY = height;
        }
        InvalidateRect(hwnd, NULL, TRUE); // Инициируем перерисовку окна
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
    wc.lpszClassName = L"MoveTextWindow";

    RegisterClass(&wc);

    // Создание окна
    HWND hwnd = CreateWindowEx(
        0,                              // Расширенный стиль окна
        L"MoveTextWindow",              // Имя класса окна
        L"Move Text Application",       // Заголовок окна
        WS_OVERLAPPEDWINDOW,            // Стиль окна
        CW_USEDEFAULT, CW_USEDEFAULT,   // Позиция и размер окна
        width, height,                       // Ширина и высота окна
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
