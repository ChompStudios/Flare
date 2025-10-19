#ifndef TABS_H
#define TABS_H

#include "../Gui/GUIElement.h"
#include <vector>
#include <string>
#include <functional>

#ifdef _WIN32
#include <windows.h>
#endif

// ---------------------------------------------
// TabPage
// Represents a single tab with its label and elements
// ---------------------------------------------
class TabPage {
public:
    std::string label;                // Tab name
    std::vector<GUIElement*> elements; // Elements inside this tab
    bool active = false;              // Is this tab currently active?

    TabPage(const std::string& name) : label(name) {}
};

// ---------------------------------------------
// Tabs
// A GUI element containing multiple TabPages
// Handles drawing, switching, and clicks
// ---------------------------------------------
class Tabs : public GUIElement {
public:
    // Constructor
    Tabs(const Rect& rect, Color bg)
        : GUIElement(rect), m_bg(bg), activeTabIndex(0) {
    }

    // Add a new tab
    void addTab(const std::string& name) {
        m_tabs.emplace_back(name);
        if (m_tabs.size() == 1)
            m_tabs[0].active = true; // First tab is active by default
    }

    // Add a GUI element to a specific tab
    void addElementToTab(size_t tabIndex, GUIElement* element) {
        if (tabIndex < m_tabs.size()) {
            m_tabs[tabIndex].elements.push_back(element);
            element->setVisible(false); // Only visible when the tab is active
        }
    }

    // Switch to a specific tab
    void switchTo(size_t index) {
        if (index >= m_tabs.size()) return;

        // Hide all elements of all tabs
        for (auto& tab : m_tabs)
            for (auto* elem : tab.elements)
                elem->setVisible(false);

        // Deactivate all tabs and activate the selected one
        for (auto& tab : m_tabs) tab.active = false;
        m_tabs[index].active = true;
        activeTabIndex = index;

        // Show elements of the active tab
        for (auto* elem : m_tabs[index].elements)
            elem->setVisible(true);

        // Call callback if set
        if (onTabChange) onTabChange(index);
    }

    // Handle clicks
    void onClick(int x, int y) override {
        int tabHeight = 30;
        int yStart = m_rect.y;

        // Check if a tab header was clicked
        for (size_t i = 0; i < m_tabs.size(); ++i) {
            int top = yStart + static_cast<int>(i * (tabHeight + 5));
            Rect tabRect{ m_rect.x, top, m_rect.width, tabHeight };
            if (x >= tabRect.x && x <= tabRect.x + tabRect.width &&
                y >= tabRect.y && y <= tabRect.y + tabRect.height) {
                switchTo(i);
                return;
            }
        }

        // Forward click to active tab's elements
        for (auto* elem : m_tabs[activeTabIndex].elements) {
            if (elem->isVisible() && elem->contains(x, y)) {
                elem->onClick(x, y);

                // Set focus if it's an InputBox
                InputBox* input = dynamic_cast<InputBox*>(elem);
                if (input) {
                    extern GUIManager guiManager;
                    guiManager.setFocusedInput(input);
                }
                return;
            }
        }
    }

    // Draw the tabs and active tab elements
    void draw(void* windowHandle = nullptr, void* display = nullptr) override {
        if (!visible || !windowHandle) return;

#ifdef _WIN32
        HDC hdc = GetDC((HWND)windowHandle);

        // Draw tab headers
        int tabHeight = 30;
        int yPos = m_rect.y + 10;
        for (size_t i = 0; i < m_tabs.size(); ++i) {
            RECT tabRect = { m_rect.x + 5, yPos, m_rect.x + m_rect.width - 5, yPos + tabHeight };
            HBRUSH brush = CreateSolidBrush(m_tabs[i].active ? RGB(200, 200, 255) : RGB(180, 180, 220));
            FillRect(hdc, &tabRect, brush);
            DeleteObject(brush);

            SetBkMode(hdc, TRANSPARENT);
            DrawTextA(hdc, m_tabs[i].label.c_str(), -1, &tabRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            yPos += tabHeight + 5;
        }

        // Draw elements of the active tab
        if (activeTabIndex < m_tabs.size()) {
            for (auto* elem : m_tabs[activeTabIndex].elements) {
                if (elem->isVisible())
                    elem->draw(windowHandle, display);
            }
        }

        ReleaseDC((HWND)windowHandle, hdc);
#endif
    }

    // Callback for when the active tab changes
    std::function<void(size_t)> onTabChange;

private:
    Color m_bg;                     // Background color
    std::vector<TabPage> m_tabs;    // All tabs
    size_t activeTabIndex;          // Currently active tab index
};

#endif // TABS_H
