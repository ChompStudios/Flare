#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include "../Gui/GUIElement.h"

// ---------------------------------------------
// Window
// Represents an application window
// Handles creation, display, events, and optional console
// ---------------------------------------------
class Window {
public:
    // Constructor
    // title : Window title
    // width : Width of the window
    // height: Height of the window
    // bg    : Background color (default white)
    Window(const std::string& title, int width, int height, Color bg = { 255, 255, 255 });

    // Destructor
    ~Window();

    // Create the actual window
    bool create();

    // Show the window
    void show();

    // Poll system/window events
    void pollEvents();

    // Check if the window should close
    bool shouldClose() const;

    // Console control (Windows only)
    void showConsole();
    void hideConsole();

#ifdef _WIN32
    // Get native window handle (HWND)
    void* getHWND() const { return m_hwnd; }
#else
    // X11-specific accessors
    void* getDisplay() const { return m_display; }
    unsigned long getX11Window() const { return m_window; }
#endif

    // Set background color
    void setBackgroundColor(Color c) { m_bg = c; }

    // Get background color
    Color getBackgroundColor() const { return m_bg; }

private:
    std::string m_title;  // Window title
    int m_width;          // Window width
    int m_height;         // Window height
    bool m_shouldClose;   // Flag indicating window closure
    Color m_bg;           // Background color

#ifdef _WIN32
    void* m_hwnd;         // Native Windows handle
#else
    void* m_display;      // X11 display pointer
    unsigned long m_window; // X11 window ID
#endif
};

#endif // WINDOW_H
