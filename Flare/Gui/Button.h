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

// ---------------------------------------------
// ButtonStyle
// Defines visual styling for buttons
// ---------------------------------------------
struct ButtonStyle {
    Color bgColor = { 220, 220, 220 };     // Default background color
    Color hoverColor = { 200, 200, 200 };  // Background color on hover
    Color textColor = { 0, 0, 0 };         // Text color
    int borderRadius = 6;                  // Corner radius (not implemented in Win32)
};

// ---------------------------------------------
// Button
// A clickable GUI element with text and callback
// ---------------------------------------------
class Button : public GUIElement {
public:
    // Constructor
    // rect    : Position and size of the button
    // text    : Button label
    // cb      : Callback function triggered on click
    // style   : Visual style of the button
    Button(const Rect& rect, const std::string& text, std::function<void()> cb, ButtonStyle style = ButtonStyle())
        : GUIElement(rect), m_text(text), m_callback(cb), m_style(style), m_hover(false)
    {
    }

    // Draw the button
    void draw(void* windowHandle = nullptr, void* display = nullptr) override {
#ifdef _WIN32
        if (!windowHandle) return;

        HDC hdc = GetDC((HWND)windowHandle);

        // Draw button background
        RECT r = { m_rect.x, m_rect.y, m_rect.x + m_rect.width, m_rect.y + m_rect.height };
        HBRUSH bgBrush = CreateSolidBrush(RGB(m_style.bgColor.r, m_style.bgColor.g, m_style.bgColor.b));
        FillRect(hdc, &r, bgBrush);
        DeleteObject(bgBrush);

        // Draw button text
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(m_style.textColor.r, m_style.textColor.g, m_style.textColor.b));
        DrawTextA(hdc, m_text.c_str(), -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

        ReleaseDC((HWND)windowHandle, hdc);
#endif
    }

    // Handle mouse click
    void onClick(int x, int y) override {
        if (contains(x, y) && m_callback)
            m_callback();
    }

    // Set button text
    void setText(const std::string& t) { m_text = t; }

    // Set button position
    void setPosition(int x, int y) { m_rect.x = x; m_rect.y = y; }

private:
    std::string m_text;                // Button label
    std::function<void()> m_callback;  // Click callback
    ButtonStyle m_style;               // Button style
    bool m_hover;                      // Hover state (not yet used)
};

#endif // BUTTON_H
