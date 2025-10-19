#ifndef SIDEPANEL_H
#define SIDEPANEL_H

#include "GUIElement.h"
#include "Button.h"
#include "../Backend systems/Tabs.h"
#include <algorithm>
#include <cmath>

class SidePanel : public GUIElement {
public:
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

    void attachButton(Button* button) {
        m_button = button;
        updateButtonLabel();
        updateButtonPosition(true);
    }

    void attachTabs(Tabs* tabs) {
        m_tabs = tabs;
    }

    void toggle() {
        m_expanded = !m_expanded;
        updateButtonLabel();
    }

    void update(int windowHeight) {
        m_rect.height = windowHeight;
        int target = m_expanded ? m_targetWidth : 0;

        float easing = 0.25f;
        float newWidth = m_currentWidth + (target - m_currentWidth) * easing;
        m_currentWidth = std::clamp(static_cast<int>(std::round(newWidth)), 0, m_targetWidth);
        m_rect.width = m_currentWidth;

        updateButtonPosition(false);

        // Update tabs position
        if (m_tabs) {
            Rect tabRect = m_tabs->getRect();

            if (m_currentWidth <= 5) { // nearly closed
                // Move tabs fully off-screen
                tabRect.x = -m_targetWidth;
                tabRect.width = m_targetWidth; // keep width for smooth animation
                tabRect.height = windowHeight;
            }
            else {
                // Show tabs and align with panel
                tabRect.x = m_rect.x;
                tabRect.width = m_currentWidth;
                tabRect.height = windowHeight;
            }

            m_tabs->setRect(tabRect);
        }
    }



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
    Color m_bg;
    bool m_expanded;
    int m_targetWidth;
    int m_currentWidth;
    Button* m_button;
    Tabs* m_tabs;

    void updateButtonPosition(bool instant) {
        if (!m_button) return;
        Rect btnRect = m_button->getRect();

        int edgeX = m_rect.x + m_currentWidth;
        btnRect.x = m_expanded ? edgeX - btnRect.width / 2
            : max(0, edgeX - btnRect.width / 4);
        btnRect.y = 20;
        m_button->setRect(btnRect);
    }

    void updateButtonLabel() {
        if (!m_button) return;
        m_button->setText(m_expanded ? "<" : ">");
    }
};

#endif // SIDEPANEL_H
