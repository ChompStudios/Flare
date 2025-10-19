# CMakeList.txt : CMake project for Flare, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Flare "Flare.cpp" "Flare.h" "Window/Window.cpp" "Window/Window.h" "Gui/GUIElement.h" "Gui/Button.h" "Gui/GUIManager.h" "Gui/InputBox.h" "Gui/SidePanel.h" "Backend systems/Tabs.h" "Gui/Label.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Flare PROPERTY CXX_STANDARD 20)
 
endif()
