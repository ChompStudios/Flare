#ifndef TABS_H
#define TABS_H

#include "../Gui/GUIElement.h"
#include <vector>
#include <string>
#include <functional>
#ifdef _WIN32
#include <windows.h>
#endif

class TabPage {
public:
    std::string label;
    std::vector<GUIElement*> elements;
    bool active = false;

    TabPage(const std::string& name) : label(name) {}
};

class Tabs : public GUIElement {
public:
    Tabs(const Rect& rect, Color bg) : GUIElement(rect), m_bg(bg), activeTabIndex(0) {}

    void addTab(const std::string& name) {
        m_tabs.emplace_back(name);
        if (m_tabs.size() == 1)
            m_tabs[0].active = true;
    }

    void addElementToTab(size_t tabIndex, GUIElement* element) {
        if (tabIndex < m_tabs.size()) {
            m_tabs[tabIndex].elements.push_back(element);
            element->setVisible(false); // only visible when active
        }
    }

    void switchTo(size_t index) {
        if (index >= m_tabs.size()) return;

        // Hide all elements
        for (auto& tab : m_tabs)
            for (auto* elem : tab.elements)
                elem->setVisible(false);

        // Activate selected tab
        for (auto& t : m_tabs) t.active = false;
        m_tabs[index].active = true;
        activeTabIndex = index;

        // Show elements for active tab
        for (auto* elem : m_tabs[index].elements)
            elem->setVisible(true);

        if (onTabChange) onTabChange(index);
    }

    void onClick(int x, int y) override {
        int tabHeight = 30;
        int yStart = m_rect.y;

        // Check tab buttons
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

    void draw(void* windowHandle = nullptr, void* display = nullptr) override {
        if (!visible || !windowHandle) return;

#ifdef _WIN32
        HDC hdc = GetDC((HWND)windowHandle);

        // Draw tab buttons
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

        // Draw active tab elements
        if (activeTabIndex < m_tabs.size()) {
            for (auto* elem : m_tabs[activeTabIndex].elements) {
                if (elem->isVisible())
                    elem->draw(windowHandle, display);
            }
        }

        ReleaseDC((HWND)windowHandle, hdc);
#endif
    }


    std::function<void(size_t)> onTabChange;

private:
    Color m_bg;
    std::vector<TabPage> m_tabs;
    size_t activeTabIndex;
};

#endif // TABS_H
