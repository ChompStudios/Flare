#include "Flare.h"


GUIManager guiManager;

int main() {
    Window window("Flare Browser", 800, 600, { 240,240,240 });
    if (!window.create()) return -1;
    window.show();

    // Side panel
    SidePanel* panel = new SidePanel({ 0,0,220,600 }, { 170,170,210 }, false);
    Button* toggleButton = new Button({ 0,20,30,30 }, ">", [panel]() { panel->toggle(); });
    panel->attachButton(toggleButton);

    // Tabs
    Tabs* tabs = new Tabs({ 0,60,220,540 }, { 160,160,200 });
    tabs->addTab("General");
    tabs->addTab("Settings");
    tabs->addTab("About");

    // General tab
    Button* b1 = new Button({ 250,100,100,30 }, "Click Me", []() { printf("General button clicked\n"); });
    tabs->addElementToTab(0, b1);

    // Settings tab
    InputBox* input = new InputBox({ 250,100,200,30 }, [](const std::string& t) { printf("Typed: %s\n", t.c_str()); });
    tabs->addElementToTab(1, input);

    Button* consoleButton = new Button({ 250,150,150,30 }, "Toggle Console", [&window]() {
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
        });
    tabs->addElementToTab(1, consoleButton);

    // About tab
    Label* lbl = new Label({ 250,100,200,30 }, "This is the About tab.");
    tabs->addElementToTab(2, lbl);

    tabs->switchTo(0);

    panel->attachTabs(tabs);

    guiManager.addElement(panel);
    guiManager.addElement(tabs);
    guiManager.addElement(toggleButton);

    // Main loop
    while (!window.shouldClose()) {
        window.pollEvents();

#ifdef _WIN32
        RECT rc;
        GetClientRect((HWND)window.getHWND(), &rc);
        int winHeight = rc.bottom - rc.top;
        panel->update(winHeight);
#endif

        guiManager.draw(window.getHWND());
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return 0;
}
