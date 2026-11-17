#define UNICODE
#define _UNICODE
#define NOMINMAX
#include <windows.h>
#include <array>
#include <cwchar>
#include <cwctype>
#include <iomanip>
#include <sstream>
#include <string>

namespace
{
    constexpr int ID_EDIT = 900;
    constexpr int ID_BTN0 = 1000;
    constexpr int BTN_W   = 60;
    constexpr int BTN_H   = 50;
    constexpr int GAP     = 10;
    constexpr int COLS    = 4;
    constexpr int ROWS    = 5;
    constexpr int EDIT_H  = 40;

    const std::array<std::array<const wchar_t*, COLS>, ROWS> g_labels = {{
        { L"C", L"←", L"(", L")" },
        { L"7", L"8", L"9", L"/" },
        { L"4", L"5", L"6", L"*" },
        { L"1", L"2", L"3", L"-" },
        { L"0", L".", L"=", L"+" }
    }};

    HWND g_hEdit = nullptr;
    HFONT g_hFont = nullptr;
    bool g_fresh = false;

    class Evaluator
    {
    public:
        enum class Status { Ok, SyntaxError, DivByZero };

        Status Evaluate(const std::wstring& expr, double& result)
        {
            p_ = expr.c_str();
            status_ = Status::Ok;
            result = ParseExpr();
            SkipSpaces();
            if (*p_ != L'\0') Fail(Status::SyntaxError);
            return status_;
        }

    private:
        const wchar_t* p_ = nullptr;
        Status status_ = Status::Ok;

        void Fail(Status s)
        {
            if (status_ == Status::Ok) status_ = s;
        }

        void SkipSpaces()
        {
            while (*p_ == L' ') ++p_;
        }

        double ParseFactor()
        {
            SkipSpaces();
            if (*p_ == L'-') { ++p_; return -ParseFactor(); }
            if (*p_ == L'+') { ++p_; return ParseFactor(); }
            if (*p_ == L'(')
            {
                ++p_;
                double v = ParseExpr();
                SkipSpaces();
                if (*p_ == L')') ++p_;
                else Fail(Status::SyntaxError);
                return v;
            }
            wchar_t* end = nullptr;
            double v = std::wcstod(p_, &end);
            if (end == p_) { Fail(Status::SyntaxError); return 0.0; }
            p_ = end;
            return v;
        }

        double ParseTerm()
        {
            double v = ParseFactor();
            for (;;)
            {
                SkipSpaces();
                if (*p_ == L'*')
                {
                    ++p_;
                    v *= ParseFactor();
                }
                else if (*p_ == L'/')
                {
                    ++p_;
                    double d = ParseFactor();
                    if (d == 0.0) { Fail(Status::DivByZero); return 0.0; }
                    v /= d;
                }
                else break;
            }
            return v;
        }

        double ParseExpr()
        {
            double v = ParseTerm();
            for (;;)
            {
                SkipSpaces();
                if (*p_ == L'+') { ++p_; v += ParseTerm(); }
                else if (*p_ == L'-') { ++p_; v -= ParseTerm(); }
                else break;
            }
            return v;
        }
    };

    std::wstring GetEditText()
    {
        int len = GetWindowTextLengthW(g_hEdit);
        std::wstring s(static_cast<size_t>(len) + 1, L'\0');
        GetWindowTextW(g_hEdit, &s[0], len + 1);
        s.resize(static_cast<size_t>(len));
        return s;
    }

    void SetEditText(const std::wstring& s)
    {
        SetWindowTextW(g_hEdit, s.c_str());
    }

    void Calculate()
    {
        double result = 0.0;
        Evaluator ev;
        switch (ev.Evaluate(GetEditText(), result))
        {
        case Evaluator::Status::Ok:
        {
            std::wostringstream os;
            os << std::setprecision(10) << result;
            SetEditText(os.str());
            break;
        }
        case Evaluator::Status::DivByZero:
            SetEditText(L"Ділення на нуль");
            break;
        default:
            SetEditText(L"Помилка");
            break;
        }
        g_fresh = true;
    }

    void AppendText(const std::wstring& s)
    {
        int len = GetWindowTextLengthW(g_hEdit);
        SendMessageW(g_hEdit, EM_SETSEL, len, len);
        SendMessageW(g_hEdit, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(s.c_str()));
    }

    void OnButton(const std::wstring& t)
    {
        if (t == L"C")
        {
            SetEditText(L"");
            g_fresh = false;
            return;
        }

        if (t == L"=")
        {
            Calculate();
            return;
        }

        if (t == L"←")
        {
            std::wstring cur = GetEditText();
            if (g_fresh) cur.clear();
            else if (!cur.empty()) cur.pop_back();
            g_fresh = false;
            SetEditText(cur);
            return;
        }

        bool isOp = (t == L"+" || t == L"-" || t == L"*" || t == L"/");
        if (g_fresh)
        {
            std::wstring cur = GetEditText();
            bool isError = cur.empty() || (cur[0] != L'-' && !std::iswdigit(cur[0]));
            if (!isOp || isError) SetEditText(L"");
            g_fresh = false;
        }
        AppendText(t);
    }

    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case WM_CREATE:
        {
            auto* cs = reinterpret_cast<CREATESTRUCTW*>(lParam);
            HINSTANCE hInst = cs->hInstance;

            g_hFont = CreateFontW(-22, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                  DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI");

            g_hEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
                                      WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_AUTOHSCROLL,
                                      GAP, GAP, COLS * BTN_W + (COLS - 1) * GAP, EDIT_H,
                                      hwnd, reinterpret_cast<HMENU>(static_cast<INT_PTR>(ID_EDIT)),
                                      hInst, nullptr);
            SendMessageW(g_hEdit, WM_SETFONT, reinterpret_cast<WPARAM>(g_hFont), TRUE);

            for (int r = 0; r < ROWS; ++r)
            {
                for (int c = 0; c < COLS; ++c)
                {
                    HWND b = CreateWindowExW(0, L"BUTTON", g_labels[r][c],
                                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                GAP + c * (BTN_W + GAP),
                                GAP + EDIT_H + GAP + r * (BTN_H + GAP),
                                BTN_W, BTN_H,
                                hwnd,
                                reinterpret_cast<HMENU>(static_cast<INT_PTR>(ID_BTN0 + r * COLS + c)),
                                hInst, nullptr);
                    SendMessageW(b, WM_SETFONT, reinterpret_cast<WPARAM>(g_hFont), TRUE);
                }
            }
            return 0;
        }

        case WM_COMMAND:
            if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) >= ID_BTN0)
            {
                int idx = LOWORD(wParam) - ID_BTN0;
                if (idx < ROWS * COLS)
                {
                    OnButton(g_labels[idx / COLS][idx % COLS]);
                    SetFocus(hwnd);
                }
            }
            return 0;

        case WM_DESTROY:
            DeleteObject(g_hFont);
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
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
    wc.lpszClassName = L"CalcWnd";
    RegisterClassW(&wc);

    RECT rc{ 0, 0,
             COLS * BTN_W + (COLS + 1) * GAP,
             EDIT_H + ROWS * BTN_H + (ROWS + 2) * GAP };
    const DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    AdjustWindowRect(&rc, style, FALSE);

    HWND hwnd = CreateWindowExW(0, wc.lpszClassName, L"Калькулятор", style,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                rc.right - rc.left, rc.bottom - rc.top,
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
