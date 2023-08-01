#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
COLORREF currentBackgroundColor = RGB(0, 0, 0);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[]  = L"GameClass";

    WNDCLASSEXW wc = {0};
    wc.cbSize        = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH) COLOR_WINDOW;

    if (!RegisterClassExW(&wc))
    {
        // If you want a more detailed error message:
        DWORD error = GetLastError();
        WCHAR errorMessage[512];
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, error,
                    0, errorMessage, sizeof(errorMessage)/sizeof(WCHAR), NULL);
        MessageBoxW(NULL, errorMessage, L"Error", MB_ICONERROR);

        return 0;
    }

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Game Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hwnd) {
        MessageBoxW(NULL, L"Failed to create window", L"Error", MB_ICONERROR);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {0};
    while (TRUE) {
        //int counter = 0;
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
            break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            Sleep(1000);
            currentBackgroundColor = (currentBackgroundColor == RGB(0, 0, 0)) ? RGB(255, 255, 255) : RGB(0, 0, 0);
            InvalidateRect(hwnd, NULL, TRUE);
        }

        
    }
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            HBRUSH brush = CreateSolidBrush(currentBackgroundColor);
            FillRect(hdc, &ps.rcPaint, brush);
            DeleteObject(brush);
            EndPaint(hwnd, &ps);
        }

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}
