#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include "../Gui/GUIElement.h"


class Window {
public:
    // Constructor now supports background color
    Window(const std::string& title, int width, int height, Color bg = { 255,255,255 });
    ~Window();

    bool create();
    void show();
    void pollEvents();
    bool shouldClose() const;

    // Console control
    void showConsole();
    void hideConsole();

#ifdef _WIN32
    void* getHWND() const { return m_hwnd; }
#else
    void* getDisplay() const { return m_display; }
    unsigned long getX11Window() const { return m_window; }
#endif

    void setBackgroundColor(Color c) { m_bg = c; }

    // Getter for background color
    Color getBackgroundColor() const { return m_bg; }

private:
    std::string m_title;
    int m_width, m_height;
    bool m_shouldClose;
    Color m_bg;

#ifdef _WIN32
    void* m_hwnd;
#else
    void* m_display;
    unsigned long m_window;
#endif
};

#endif // WINDOW_H
