#define UNICODE
#define _UNICODE
#define NOMINMAX
#include <windows.h>
#include <algorithm>
#include <random>

namespace
{
    constexpr int ID_BUTTON = 1001;
    constexpr int BTN_W = 120;
    constexpr int BTN_H = 40;

    HWND g_hBtn = nullptr;
    std::mt19937 g_rng{ std::random_device{}() };

    void MoveButtonRandomly(HWND hwnd)
    {
        RECT rc;
        GetClientRect(hwnd, &rc);

        int maxX = std::max(0, static_cast<int>(rc.right) - BTN_W);
        int maxY = std::max(0, static_cast<int>(rc.bottom) - BTN_H);

        int x = std::uniform_int_distribution<int>(0, maxX)(g_rng);
        int y = std::uniform_int_distribution<int>(0, maxY)(g_rng);

        SetWindowPos(g_hBtn, nullptr, x, y, BTN_W, BTN_H, SWP_NOZORDER);
    }

    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case WM_CREATE:
        {
            auto* cs = reinterpret_cast<CREATESTRUCTW*>(lParam);
            g_hBtn = CreateWindowExW(0, L"BUTTON", L"Натисни мене",
                                     WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                     100, 100, BTN_W, BTN_H,
                                     hwnd, reinterpret_cast<HMENU>(static_cast<INT_PTR>(ID_BUTTON)),
                                     cs->hInstance, nullptr);
            return 0;
        }

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_BUTTON && HIWORD(wParam) == BN_CLICKED)
                MoveButtonRandomly(hwnd);
            return 0;

        case WM_SIZE:
        {
            if (!g_hBtn) return 0;
            RECT rc, br;
            GetClientRect(hwnd, &rc);
            GetWindowRect(g_hBtn, &br);
            MapWindowPoints(HWND_DESKTOP, hwnd, reinterpret_cast<LPPOINT>(&br), 2);
            if (br.right > rc.right || br.bottom > rc.bottom)
                MoveButtonRandomly(hwnd);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int nShow)
{
    WNDCLASSW wc{};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = L"RandomButtonWnd";
    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(0, wc.lpszClassName, L"Кнопка-втікачка",
                                WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT, CW_USEDEFAULT, 500, 400,
                                nullptr, nullptr, hInst, nullptr);
    if (!hwnd) return 1;
    ShowWindow(hwnd, nShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return static_cast<int>(msg.wParam);
}
