#include "Window.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <windowsx.h>
#include "../Gui/GUIManager.h"
extern GUIManager guiManager;

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

// Forward declare helper to get Window pointer from hwnd
inline Window* GetWindowPtr(HWND hwnd) {
    return (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Window* window = GetWindowPtr(hwnd);

    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        if (window) {
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            Color bg = window->getBackgroundColor();
            HBRUSH brush = CreateSolidBrush(RGB(bg.r, bg.g, bg.b));
            FillRect(hdc, &clientRect, brush);
            DeleteObject(brush);
        }

        guiManager.draw(hwnd);

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_LBUTTONDOWN: {
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);
        guiManager.click(x, y);
        return 0;
    }

    case WM_CHAR: {
        char c = (char)wParam;
        guiManager.keyPress(c);
        return 0;
    }

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

Window::Window(const std::string& title, int width, int height, Color bg)
    : m_title(title), m_width(width), m_height(height),
    m_shouldClose(false), m_hwnd(nullptr), m_bg(bg) {
    FreeConsole();
}

Window::~Window() {
    if (m_hwnd) DestroyWindow((HWND)m_hwnd);
}

bool Window::create() {
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "SimpleWindowClass";

    if (!RegisterClass(&wc)) {
        std::cerr << "Failed to register window class\n";
        return false;
    }

    HWND hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        m_title.c_str(),
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, m_width, m_height,
        nullptr, nullptr, hInstance, nullptr
    );

    if (!hwnd) {
        std::cerr << "Failed to create window\n";
        return false;
    }

    m_hwnd = hwnd;
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this); // store pointer
    return true;
}

void Window::show() { ShowWindow((HWND)m_hwnd, SW_SHOW); }

void Window::pollEvents() {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) m_shouldClose = true;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

bool Window::shouldClose() const { return m_shouldClose; }

void Window::showConsole() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
}

void Window::hideConsole() { FreeConsole(); }

#else // Linux / X11

#include <X11/Xlib.h>
#include <unistd.h>
#include "../Gui/GUIManager.h"
extern GUIManager guiManager;

Window::Window(const std::string& title, int width, int height, Color bg)
    : m_title(title), m_width(width), m_height(height),
    m_shouldClose(false), m_display(nullptr), m_window(0), m_bg(bg) {
}

Window::~Window() {
    if (m_display) {
        XDestroyWindow((Display*)m_display, m_window);
        XCloseDisplay((Display*)m_display);
    }
}

bool Window::create() {
    Display* display = XOpenDisplay(nullptr);
    if (!display) {
        std::cerr << "Cannot open X display\n";
        return false;
    }

    int screen = DefaultScreen(display);
    ::Window root = RootWindow(display, screen);

    ::Window win = XCreateSimpleWindow(display, root, 10, 10, m_width, m_height, 1,
        BlackPixel(display, screen), WhitePixel(display, screen));

    XStoreName(display, win, m_title.c_str());
    XSelectInput(display, win, ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask);

    m_display = display;
    m_window = win;
    return true;
}

void Window::show() {
    XMapWindow((Display*)m_display, m_window);
    XFlush((Display*)m_display);
}

void Window::pollEvents() {
    if (!m_display) return;

    while (XPending((Display*)m_display)) {
        XEvent ev;
        XNextEvent((Display*)m_display, &ev);

        if (ev.type == DestroyNotify) m_shouldClose = true;

        if (ev.type == ButtonPress) {
            int x = ev.xbutton.x;
            int y = ev.xbutton.y;
            guiManager.click(x, y);
        }

        if (ev.type == KeyPress) {
            char c = XLookupKeysym(&ev.xkey, 0);
            guiManager.keyPress(c);
        }

        if (ev.type == Expose) {
            Display* display = (Display*)m_display;
            GC gc = DefaultGC(display, DefaultScreen(display));
            XSetForeground(display, gc, (m_bg.r << 16) | (m_bg.g << 8) | m_bg.b);
            XFillRectangle(display, m_window, gc, 0, 0, m_width, m_height);

            guiManager.draw(nullptr, m_display);
        }
    }
}

bool Window::shouldClose() const { return m_shouldClose; }

#endif
