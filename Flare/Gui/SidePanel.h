#ifndef SIDEPANEL_H
#define SIDEPANEL_H

#include "GUIElement.h"
#include "Button.h"
#include "../Backend systems/Tabs.h"
#include <algorithm>
#include <cmath>

// ---------------------------------------------
// SidePanel
// A collapsible panel that can hold buttons and tabs.
// Supports smooth width animation when toggled.
// ---------------------------------------------
class SidePanel : public GUIElement {
public:
    // Constructor
    // rect      : Position and size of the panel
    // bg        : Background color
    // expanded  : Initial expanded state
    SidePanel(const Rect& rect, Color bg, bool expanded = false)
        : GUIElement(rect),
        m_bg(bg),
        m_expanded(expanded),
        m_targetWidth(rect.width),
        m_currentWidth(expanded ? rect.width : 0),
        m_button(nullptr),
        m_tabs(nullptr)
    {
    }

    // Attach a toggle button to the panel
    void attachButton(Button* button) {
        m_button = button;
        updateButtonLabel();
        updateButtonPosition(true);
    }

    // Attach a Tabs object to the panel
    void attachTabs(Tabs* tabs) {
        m_tabs = tabs;
    }

    // Toggle expanded/collapsed state
    void toggle() {
        m_expanded = !m_expanded;
        updateButtonLabel();
    }

    // Update panel dimensions and positions each frame
    void update(int windowHeight) {
        m_rect.height = windowHeight;

        int targetWidth = m_expanded ? m_targetWidth : 0;
        const float easing = 0.25f;
        float newWidth = m_currentWidth + (targetWidth - m_currentWidth) * easing;
        m_currentWidth = std::clamp(static_cast<int>(std::round(newWidth)), 0, m_targetWidth);
        m_rect.width = m_currentWidth;

        // Update button position
        updateButtonPosition(false);

        // Update attached tabs position
        if (m_tabs) {
            Rect tabRect = m_tabs->getRect();

            if (m_currentWidth <= 5) { // nearly closed
                tabRect.x = -m_targetWidth;  // move off-screen
                tabRect.width = m_targetWidth; // preserve width for animation
                tabRect.height = windowHeight;
            }
            else {
                tabRect.x = m_rect.x;
                tabRect.width = m_currentWidth;
                tabRect.height = windowHeight;
            }

            m_tabs->setRect(tabRect);
        }
    }

    // Draw the panel
    void draw(void* windowHandle = nullptr, void* display = nullptr) override {
#ifdef _WIN32
        if (!windowHandle || m_currentWidth <= 0) return;

        HDC hdc = GetDC((HWND)windowHandle);

        HBRUSH brush = CreateSolidBrush(RGB(m_bg.r, m_bg.g, m_bg.b));
        RECT r = { m_rect.x, m_rect.y, m_rect.x + m_rect.width, m_rect.y + m_rect.height };
        FillRect(hdc, &r, brush);
        DeleteObject(brush);

        ReleaseDC((HWND)windowHandle, hdc);
#endif
    }

private:
    Color m_bg;          // Background color
    bool m_expanded;     // Panel expanded state
    int m_targetWidth;   // Full width of the panel
    int m_currentWidth;  // Current width for animation
    Button* m_button;    // Attached toggle button
    Tabs* m_tabs;        // Attached tabs

    // Update button position based on current panel width
    void updateButtonPosition(bool instant) {
        if (!m_button) return;

        Rect btnRect = m_button->getRect();
        int edgeX = m_rect.x + m_currentWidth;

        btnRect.x = m_expanded
            ? edgeX - btnRect.width / 2
            : max(0, edgeX - btnRect.width / 4);
        btnRect.y = 20;

        m_button->setRect(btnRect);
    }

    // Update button label based on panel state
    void updateButtonLabel() {
        if (!m_button) return;
        m_button->setText(m_expanded ? "<" : ">");
    }
};

#endif // SIDEPANEL_H
