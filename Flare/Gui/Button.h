#ifndef BUTTON_H
#define BUTTON_H

#include "GUIElement.h"
#include <string>
#include <functional>

#ifdef _WIN32
#include <windows.h>
#else
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

struct ButtonStyle {
    Color bgColor = { 220, 220, 220 };
    Color hoverColor = { 200, 200, 200 };
    Color textColor = { 0, 0, 0 };
    int borderRadius = 6;
};

class Button : public GUIElement {
public:
    Button(const Rect& rect, const std::string& text, std::function<void()> cb, ButtonStyle style = ButtonStyle())
        : GUIElement(rect), m_text(text), m_callback(cb), m_style(style), m_hover(false) {
    }

    void draw(void* windowHandle = nullptr, void* display = nullptr) override {
#ifdef _WIN32
        if (!windowHandle) return;
        HDC hdc = GetDC((HWND)windowHandle);

        RECT r = { m_rect.x, m_rect.y, m_rect.x + m_rect.width, m_rect.y + m_rect.height };
        HBRUSH bgBrush = CreateSolidBrush(RGB(m_style.bgColor.r, m_style.bgColor.g, m_style.bgColor.b));
        FillRect(hdc, &r, bgBrush);
        DeleteObject(bgBrush);

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(m_style.textColor.r, m_style.textColor.g, m_style.textColor.b));
        DrawTextA(hdc, m_text.c_str(), -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

        ReleaseDC((HWND)windowHandle, hdc);
#endif
    }

    void onClick(int x, int y) override {
        if (contains(x, y) && m_callback) m_callback();
    }

    void setText(const std::string& t) { m_text = t; }
    void setPosition(int x, int y) { m_rect.x = x; m_rect.y = y; }

private:
    std::string m_text;
    std::function<void()> m_callback;
    ButtonStyle m_style;
    bool m_hover;
};

#endif
