#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
COLORREF currentBackgroundColor = RGB(0, 0, 0);
UINT WIDTH = 800;
UINT HEIGHT = 600;

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
        CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT,
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
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
            break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
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
            HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, L"/data/ship.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
    
            HDC hdcMem = CreateCompatibleDC(hdc);
            HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hBitmap);

            BITMAP bitmap;
            GetObject(hBitmap, sizeof(bitmap), &bitmap);

            BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

            SelectObject(hdcMem, hbmOld);
            DeleteDC(hdcMem);

            EndPaint(hwnd, &ps);
            break;
        }
        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_SPACE:
                    currentBackgroundColor = (currentBackgroundColor == RGB(0, 0, 0)) ? RGB(255, 255, 255) : RGB(0, 0, 0);
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
            }
            break;
        }
        case WM_LBUTTONDOWN:
        {
            OutputDebugStringW(L"L_BUTTON!\n");
            currentBackgroundColor = (currentBackgroundColor == RGB(255, 0, 0)) ? RGB(0, 0, 0) : RGB(255, 0, 0);
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
        case WM_RBUTTONDOWN:
        {
            OutputDebugStringW(L"R_BUTTON!\n");
            currentBackgroundColor = (currentBackgroundColor == RGB(255, 255, 0)) ? RGB(0, 0, 0) : RGB(255, 255, 0);
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
        case WM_MOUSEMOVE:
        {
            int mouseX = LOWORD(lParam);
            int mouseY = HIWORD(lParam);
            wprintf(L"Mouse moved to %d, %d\n", mouseX, mouseY);
            break;
        }

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}
