#ifndef INPUTBOX_H
#define INPUTBOX_H

#include "GUIElement.h"
#include <string>
#include <functional>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#else
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

struct InputStyle {
    Color bgColor = { 255, 255, 255 };
    Color textColor = { 0, 0, 0 };
    Color borderColor = { 0, 0, 0 };
    int borderRadius = 8;
    int borderThickness = 1;
};

class InputBox : public GUIElement {
public:
    InputBox(const Rect& rect, std::function<void(const std::string&)> cb, InputStyle style = InputStyle())
        : GUIElement(rect), m_callback(cb), m_style(style),
        m_focused(false), m_text(""), m_lastBlink(std::chrono::steady_clock::now()), m_cursorVisible(true) {
    }

    void draw(void* windowHandle = nullptr, void* display = nullptr) override {
#ifdef _WIN32
        if (!windowHandle) return;
        HDC hdc = GetDC((HWND)windowHandle);

        Color bg = m_style.bgColor;
        HBRUSH brush = CreateSolidBrush(RGB(bg.r, bg.g, bg.b));
        HPEN pen = CreatePen(PS_SOLID, m_style.borderThickness, RGB(m_style.borderColor.r, m_style.borderColor.g, m_style.borderColor.b));

        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);

        RoundRect(hdc, m_rect.x, m_rect.y, m_rect.x + m_rect.width, m_rect.y + m_rect.height,
            m_style.borderRadius, m_style.borderRadius);

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(m_style.textColor.r, m_style.textColor.g, m_style.textColor.b));
        RECT r = { m_rect.x + 5, m_rect.y + 5, m_rect.x + m_rect.width - 5, m_rect.y + m_rect.height - 5 };
        DrawTextA(hdc, m_text.c_str(), -1, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

        updateCursorBlink();
        if (m_focused && m_cursorVisible) {
            SIZE sz;
            GetTextExtentPoint32A(hdc, m_text.c_str(), m_text.size(), &sz);
            MoveToEx(hdc, m_rect.x + 8 + sz.cx, m_rect.y + 5, nullptr);
            LineTo(hdc, m_rect.x + 8 + sz.cx, m_rect.y + m_rect.height - 5);
        }

        SelectObject(hdc, oldBrush);
        SelectObject(hdc, oldPen);
        DeleteObject(brush);
        DeleteObject(pen);
        ReleaseDC((HWND)windowHandle, hdc);
#endif
    }

    void onClick(int x, int y) override {
        m_focused = contains(x, y);
    }

    void onKeyPress(char c) {
        if (!m_focused) return;
        if (c == '\b') {
            if (!m_text.empty()) m_text.pop_back();
        }
        else if (c == '\r' || c == '\n') {
            if (m_callback) m_callback(m_text);
        }
        else {
            m_text.push_back(c);
        }
    }

private:
    void updateCursorBlink() {
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastBlink).count() > 500) {
            m_cursorVisible = !m_cursorVisible;
            m_lastBlink = now;
        }
    }

    std::string m_text;
    std::function<void(const std::string&)> m_callback;
    InputStyle m_style;
    bool m_focused;
    bool m_cursorVisible;
    std::chrono::steady_clock::time_point m_lastBlink;
};

#endif
