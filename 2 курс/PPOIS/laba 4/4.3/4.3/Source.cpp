#include <windows.h>
#include <vector>

// ���������� ����������
std::vector<POINT> points; // ������ ��� �������� ��������� �����

// ��������� �������
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// ����� ����� ��� ����������
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

// ���������� ��������� ��� �������� ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch (message)
    {
    case WM_LBUTTONDOWN: // ��������� ������� ����� ������� ����
        POINT point;
        point.x = LOWORD(lParam);
        point.y = HIWORD(lParam);
        points.push_back(point); // ���������� ��������� ����� � �������
        InvalidateRect(hWnd, NULL, TRUE); // ����� ����������� ����
        break;
    case WM_RBUTTONDOWN: // ��������� ������� ������ ������� ����
        // ����� ����� �������� ��� ��� ������ ����������� � �������������� ��������� �� ������� points
        break;
    case WM_PAINT: // ��������� ��������� � ����������� ����
        hdc = BeginPaint(hWnd, &ps);
        // ����� ����� �������� ��� ��� ��������� ����� � ���������� �� ������� �������
        for (size_t i = 0; i < points.size(); ++i)
        {
            Ellipse(hdc, points[i].x - 3, points[i].y - 3, points[i].x + 3, points[i].y + 3); // ��������� �����
            if (i > 0)
            {
                MoveToEx(hdc, points[i - 1].x, points[i - 1].y, NULL);
                LineTo(hdc, points[i].x, points[i].y); // ���������� ����� �������
            }
        }
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY: // ��������� ��������� �� ����������� ����
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
