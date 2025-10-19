#ifndef LABEL_H
#define LABEL_H

#include "../Gui/GUIElement.h"
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

// ---------------------------------------------
// Label
// A read-only text element for the GUI.
// Inherits from GUIElement.
// Always visible by default unless explicitly hidden.
// ---------------------------------------------
class Label : public GUIElement {
public:
    // Constructor
    // rect  : Position and size of the label
    // text  : The label text (read-only)
    // color : Text color (default black)
    Label(const Rect& rect, const std::string& text, Color color = { 0, 0, 0 })
        : GUIElement(rect), m_text(text), m_color(color) {
        visible = true; // always visible by default
    }

    // -----------------------------
    // Accessors
    // -----------------------------

    // Get the label text (read-only)
    std::string getText() const { return m_text; }

    // Set the text color
    void setColor(Color color) { m_color = color; }

    // Get the current text color
    Color getColor() const { return m_color; }

    // -----------------------------
    // Drawing
    // -----------------------------
    // Draw the label on the window
    void draw(void* windowHandle = nullptr, void* display = nullptr) override {
        if (!visible || !windowHandle) return;

#ifdef _WIN32
        HDC hdc = GetDC((HWND)windowHandle);

        // Transparent background
        SetBkMode(hdc, TRANSPARENT);

        // Set text color
        SetTextColor(hdc, RGB(m_color.r, m_color.g, m_color.b));

        // Draw the text within the rectangle
        RECT textRect = { m_rect.x, m_rect.y, m_rect.x + m_rect.width, m_rect.y + m_rect.height };
        DrawTextA(hdc, m_text.c_str(), -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        ReleaseDC((HWND)windowHandle, hdc);
#endif
    }

private:
    const std::string m_text; // Read-only text content
    Color m_color;            // Current text color
};

#endif // LABEL_H
