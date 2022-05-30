#include "App.h"

#include "iWindow.h"
#include "GameMgr.h"
#include "InputMgr.h"

static int win_border_width, win_border_height;

HWND hWnd;
HDC hDC;

bool runApp;
DWORD prevTime;
bool* keys;

LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    ULONG_PTR token = iGraphics::startGdiplus();

    WCHAR szTitle[20] = L"Title Name";
    WCHAR szWindowClass[20] = L"Class Name";

    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = wndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_METALSLUG));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_PS);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassExW(&wcex);

    //Monitor Size
    int mW = GetSystemMetrics(SM_CXSCREEN);
    int mH = GetSystemMetrics(SM_CYSCREEN);

    float wndWidth = mW * 2 / 3;
    float wndHeight = mH * 2 / 3;

    hWnd = CreateWindowW(szWindowClass, szTitle,
        WS_OVERLAPPEDWINDOW,
        wndWidth / 4, wndHeight / 4,// x, y
        wndWidth, wndHeight,// w, h
        nullptr, nullptr, hInstance, nullptr);

    hDC = GetDC(hWnd);
    loadOpenGL(hDC);

    //Adjust wndSize with convert Ratio
    int wH = wndHeight + win_border_height;         //Monitor 2/3 size + outline pixel value
    int wW = wndHeight * devSize.width / devSize.height + win_border_width;

    MoveWindow(hWnd, (mW - wW) / 2, (mH - wH) / 2, wW, wH, TRUE);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    prevTime = GetTickCount();
    keys = new bool[128];
    memset(keys, false, sizeof(bool) * 128);
    srand(time(NULL));

    loadGame();

    runApp = true;
    MSG msg;
    while (runApp)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            DWORD now = GetTickCount64();
            float delta = (now - prevTime) / 1000.f;
            prevTime = now;

            drawOpenGL(delta, drawGame);
        }
    }

    freeGame();

    freeOpenGL();
    ReleaseDC(hWnd, hDC);
    DestroyWindow(hWnd);

    delete keys;
    iGraphics::stopGdiplus(token);
    return (int)msg.wParam;
}

iPoint convertCoordinate(float x, float y)
{
    RECT rt;
    GetClientRect(hWnd, &rt);
    x -= rt.left;
    y -= rt.top;

    return iPointMake(
        (x - viewport.origin.x) / viewport.size.width * devSize.width,
        (y - viewport.origin.y) / viewport.size.height * devSize.height);
}

LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
    {
        if (keys[wParam] == false)
        {
            setKeyboard(true, wParam);
            keys[wParam] = true;
        }
        break;
    }
    case WM_KEYUP:
    {
        setKeyboard(false, wParam);
        keys[wParam] = false;
        break;
    }
    case WM_LBUTTONDOWN:
    {
        keyGame(iKeyStateBegan, convertCoordinate(LOWORD(lParam), HIWORD(lParam)));
        break;
    }
    case WM_MOUSEMOVE:
    {
        iPoint c = convertCoordinate(LOWORD(lParam), HIWORD(lParam));
        keyGame(iKeyStateMoved, c);
        break;
    }
    case WM_LBUTTONUP:
    {
        keyGame(iKeyStateEnded, convertCoordinate(LOWORD(lParam), HIWORD(lParam)));
        break;
    }
    case WM_CREATE:
    {
        if (hWnd == NULL)
            hWnd = hwnd;

        RECT rt0, rt1;
        GetWindowRect(hWnd, &rt0);
        GetClientRect(hWnd, &rt1);
        win_border_width = (rt0.right - rt0.left) - (rt1.right - rt1.left);
        win_border_height = (rt0.bottom - rt0.top) - (rt1.bottom - rt1.top);
        break;
    }
    case WM_GETMINMAXINFO:
    {
        float height = GetSystemMetrics(SM_CYSCREEN) / 3;
        int width = height * devSize.width / devSize.height;

        MINMAXINFO* info = (MINMAXINFO*)lParam;
        info->ptMinTrackSize.x = width + win_border_width;
        info->ptMinTrackSize.y = height + win_border_height;
        break;
    }
    case WM_MOVE:
    case WM_SIZING:
    {
        RECT& rect = *reinterpret_cast<LPRECT>(lParam);
        enforceResolution(int(wParam), rect, win_border_width, win_border_height);
        break;
    }
    case WM_SIZE:
    {
        resizeOpenGL(LOWORD(lParam), HIWORD(lParam));
        break;
    }
    case WM_CLOSE:
    {
        int result = MessageBox(NULL, L"종료할래?", L"Exit", MB_YESNO);
        if (result == IDYES)
        {
            runApp = false;
        }
        else
        {
            return 0;
        }
        break;
    }
    case WM_DESTROY:        
        PostQuitMessage(0);
        return 0;// break;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}
