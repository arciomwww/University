#include <windows.h> 
#include <wchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

wchar_t szWindowStyle[] = L"myWindowStyle";

int mouseX = 0;
int mouseY = 0;

int WINAPI WinMain(HINSTANCE hInst, 
    HINSTANCE hPreInst,
    LPSTR lpszCmdLine, 
    int nCmdShow) 
{
    HWND hWnd; 
    MSG lpMsg; 
    WNDCLASS wcApp; 
    wcApp.lpszClassName = szWindowStyle;
    wcApp.hInstance = hInst; 
    wcApp.lpfnWndProc = WndProc;  
    wcApp.hCursor = LoadCursor(NULL, IDC_ARROW); 
    wcApp.hIcon = 0; 
    wcApp.lpszMenuName = 0; 
    wcApp.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); 
    wcApp.style = CS_HREDRAW | CS_VREDRAW; 
    wcApp.cbClsExtra = 0;        
    wcApp.cbWndExtra = 0;  

    if (!RegisterClass(&wcApp))    
        return  0;

    hWnd = CreateWindow(szWindowStyle,
        L"ÒÈÏÎÂÎÉ ÊÀÐÊÀÑ Windows-ïðèëîæåíèÿ ... ", 
        WS_OVERLAPPEDWINDOW, 
        CW_USEDEFAULT, 
        CW_USEDEFAULT, 
        CW_USEDEFAULT, 
        CW_USEDEFAULT, 
        (HWND)NULL, 
        (HMENU)NULL, 
        hInst,
        NULL); 
    ShowWindow(hWnd, nCmdShow); 
    UpdateWindow(hWnd); 
    while (GetMessage(&lpMsg, NULL, 0, 0))
    {
        TranslateMessage(&lpMsg);
        DispatchMessage(&lpMsg); 
    }
    return (lpMsg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd,
    UINT messg, 
    WPARAM wParam, LPARAM lParam)
{
    HDC hdc;  
    PAINTSTRUCT ps; 
    switch (messg)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        
        wchar_t text[50];
        swprintf(text, 50, L"X = %d, Y = %d", mouseX, mouseY);
        TextOut(hdc, mouseX, mouseY, text, wcslen(text)); 
        ValidateRect(hWnd, NULL);
        EndPaint(hWnd, &ps);
        break;

    case WM_LBUTTONDOWN: 
        mouseX = LOWORD(lParam); 
        mouseY = HIWORD(lParam);
        InvalidateRect(hWnd, NULL, TRUE); 
        break;
    case WM_RBUTTONDOWN: 
        InvalidateRect(hWnd, NULL, TRUE); 
        break;
    case WM_DESTROY: 
        PostQuitMessage(0);
        break;
    default:
        return (DefWindowProc(hWnd, messg, wParam, lParam));
    }
    return 0;
}