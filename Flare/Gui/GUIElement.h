#ifndef GUIELEMENT_H
#define GUIELEMENT_H

// ---------------------------------------------
// Rect
// Defines a rectangle for positioning and sizing GUI elements
// ---------------------------------------------
struct Rect {
    int x, y;
    int width, height;
};

// ---------------------------------------------
// Color
// Represents an RGB color
// ---------------------------------------------
struct Color {
    unsigned char r, g, b;
};

// ---------------------------------------------
// GUIElement
// Base class for all GUI elements
// Provides position, visibility, drawing, and click handling
// ---------------------------------------------
class GUIElement {
public:
    // Constructor
    GUIElement(const Rect& rect) : m_rect(rect), visible(true) {}
    virtual ~GUIElement() = default;

    // Pure virtual draw method to be implemented by derived classes
    virtual void draw(void* windowHandle = nullptr, void* display = nullptr) = 0;

    // Optional click handler
    virtual void onClick(int x, int y) {}

    // Check if a point (x, y) is within the element's rectangle
    bool contains(int x, int y) const {
        return x >= m_rect.x && x <= m_rect.x + m_rect.width &&
            y >= m_rect.y && y <= m_rect.y + m_rect.height;
    }

    // Set or get the element's rectangle
    void setRect(const Rect& r) { m_rect = r; }
    Rect getRect() const { return m_rect; }

    // Set or get visibility
    void setVisible(bool v) { visible = v; }
    bool isVisible() const { return visible; }

protected:
    Rect m_rect;   // Position and size
    bool visible;  // Visibility flag
};

#endif // GUIELEMENT_H
