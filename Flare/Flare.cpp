#include "Flare.h"
#include <thread>
#include <chrono>
#include <cstdio>

GUIManager guiManager;

int main() {
    // -------------------------------
    // Create the main application window
    // -------------------------------
    Window window("Flare Browser", 800, 600, { 240, 240, 240 });
    if (!window.create()) return -1;
    window.show();

    // -------------------------------
    // Create the side panel
    // -------------------------------
    SidePanel* panel = new SidePanel({ 0, 0, 220, 600 }, { 170, 170, 210 }, false);

    // Toggle button for showing/hiding the panel
    Button* toggleButton = new Button(
        { 0, 20, 30, 30 },
        ">",
        [panel]() { panel->toggle(); }
    );
    panel->attachButton(toggleButton);

    // -------------------------------
    // Create Tabs inside the side panel
    // -------------------------------
    Tabs* tabs = new Tabs({ 0, 60, 220, 540 }, { 160, 160, 200 });
    tabs->addTab("General");
    tabs->addTab("Settings");
    tabs->addTab("About");

    // -------------------------------
    // General Tab Elements
    // -------------------------------
    Button* generalButton = new Button(
        { 250, 100, 100, 30 },
        "Click Me",
        []() { printf("General button clicked\n"); }
    );
    tabs->addElementToTab(0, generalButton);

    // -------------------------------
    // Settings Tab Elements
    // -------------------------------
    // Input box for user input
    InputBox* inputBox = new InputBox(
        { 250, 100, 200, 30 },
        [](const std::string& text) { printf("Typed: %s\n", text.c_str()); }
    );
    tabs->addElementToTab(1, inputBox);

    // Button to toggle the console window
    Button* consoleButton = new Button(
        { 250, 150, 150, 30 },
        "Toggle Console",
        [&window]() {
            static bool consoleOpened = false;

            if (!consoleOpened) {
                window.showConsole();
                FILE* f;
                freopen_s(&f, "CONOUT$", "w", stdout);
                freopen_s(&f, "CONIN$", "r", stdin);
                consoleOpened = true;
            }
            else {
                window.hideConsole();
                consoleOpened = false;
            }
        }
    );
    tabs->addElementToTab(1, consoleButton);

    // -------------------------------
    // About Tab Elements
    // -------------------------------
    Label* aboutLabel = new Label(
        { 250, 100, 200, 30 },
        "This is the About tab."
    );
    tabs->addElementToTab(2, aboutLabel);

    // -------------------------------
    // Initialize Tabs
    // -------------------------------
    tabs->switchTo(0); // Show General tab by default
    panel->attachTabs(tabs);

    // -------------------------------
    // Add elements to GUI manager
    // -------------------------------
    guiManager.addElement(panel);
    guiManager.addElement(tabs);
    guiManager.addElement(toggleButton);

    // -------------------------------
    // Main application loop
    // -------------------------------
    while (!window.shouldClose()) {
        window.pollEvents();

        // Update panel height on Windows
#ifdef _WIN32
        RECT rc;
        GetClientRect((HWND)window.getHWND(), &rc);
        int winHeight = rc.bottom - rc.top;
        panel->update(winHeight);
#endif

        // Draw all GUI elements
        guiManager.draw(window.getHWND());

        // Limit frame rate (~60 FPS)
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return 0;
}
