#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include "GUIElement.h"
#include "InputBox.h"
#include <vector>
#include <windows.h>

// ---------------------------------------------
// GUIManager
// Manages GUI elements, drawing, input, and focus
// ---------------------------------------------
class GUIManager {
public:
    // Add a GUI element to the manager
    void addElement(GUIElement* element) {
        m_elements.push_back(element);
    }

    // Draw all managed elements
    void draw(void* windowHandle = nullptr, void* display = nullptr) {
#ifdef _WIN32
        // Clear background
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

        // Draw each element
        for (auto& e : m_elements)
            e->draw(windowHandle, display);
    }

    // Handle mouse click at (x, y)
    void click(int x, int y) {
        focusedElement = nullptr;

        for (auto& e : m_elements) {
            e->onClick(x, y);

            // If clicked element is an InputBox, set focus
            InputBox* input = dynamic_cast<InputBox*>(e);
            if (input && input->contains(x, y)) {
                focusedElement = input;
            }
        }
    }

    // Handle keyboard input
    void keyPress(char c) {
        if (focusedElement)
            focusedElement->onKeyPress(c);
    }

    // Set the currently focused InputBox
    void setFocusedInput(InputBox* input) { focusedElement = input; }

private:
    std::vector<GUIElement*> m_elements; // All managed elements
    InputBox* focusedElement = nullptr;  // Currently focused input box
};

#endif // GUIMANAGER_H
