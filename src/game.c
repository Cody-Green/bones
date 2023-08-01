#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
        int counter = 0;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            ++counter;
            if (counter > 1000)
            {
                counter = 0;
                wc.hbrBackground = (wc.hbrBackground == (HBRUSH)COLOR_WINDOW) ? 
                    (HBRUSH)COLOR_WINDOW + 1 : (HBRUSH)COLOR_WINDOW;
                InvalidateRect(hwnd, NULL, TRUE);
            }
        }

        if (msg.message == WM_QUIT) {
            break;
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

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}
