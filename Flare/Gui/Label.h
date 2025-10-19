#ifndef LABEL_H
#define LABEL_H

#include "../Gui/GUIElement.h"
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif

class Label : public GUIElement {
public:
    Label(const Rect& rect, const std::string& text, Color color = { 0,0,0 })
        : GUIElement(rect), m_text(text), m_color(color) {
        visible = true; // always visible by default
    }

    // No setter for text, since it's static
    std::string getText() const { return m_text; }
    void setColor(Color color) { m_color = color; }
    Color getColor() const { return m_color; }

    void draw(void* windowHandle = nullptr, void* display = nullptr) override {
        if (!visible || !windowHandle) return;

#ifdef _WIN32
        HDC hdc = GetDC((HWND)windowHandle);

        SetBkMode(hdc, TRANSPARENT); // no background
        SetTextColor(hdc, RGB(m_color.r, m_color.g, m_color.b));

        RECT textRect = { m_rect.x, m_rect.y, m_rect.x + m_rect.width, m_rect.y + m_rect.height };
        DrawTextA(hdc, m_text.c_str(), -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        ReleaseDC((HWND)windowHandle, hdc);
#endif
    }

private:
    const std::string m_text; // read-only text
    Color m_color;
};

#endif // LABEL_H
