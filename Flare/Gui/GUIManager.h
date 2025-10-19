#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include "GUIElement.h"
#include "InputBox.h"
#include <vector>

class GUIManager {
public:
    void addElement(GUIElement* e) {
        m_elements.push_back(e);
    }

    void draw(void* windowHandle = nullptr, void* display = nullptr) {
#ifdef _WIN32
        if (windowHandle) {
            HDC hdc = GetDC((HWND)windowHandle);
            RECT rc;
            GetClientRect((HWND)windowHandle, &rc);

            HBRUSH bg = CreateSolidBrush(RGB(240, 240, 240));
            FillRect(hdc, &rc, bg);
            DeleteObject(bg);
            ReleaseDC((HWND)windowHandle, hdc);
        }
#endif
        for (auto& e : m_elements)
            e->draw(windowHandle, display);
    }

    void click(int x, int y) {
        focusedElement = nullptr;

        for (auto& e : m_elements) {
            e->onClick(x, y);

            InputBox* input = dynamic_cast<InputBox*>(e);
            if (input && input->contains(x, y)) {
                focusedElement = input;
            }
        }
    }

    void keyPress(char c) {
        if (focusedElement)
            focusedElement->onKeyPress(c);
    }

    void setFocusedInput(InputBox* input) { focusedElement = input; }

private:
    std::vector<GUIElement*> m_elements;
    InputBox* focusedElement = nullptr;
};

#endif // GUIMANAGER_H
