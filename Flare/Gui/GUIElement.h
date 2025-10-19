#ifndef GUIELEMENT_H
#define GUIELEMENT_H

struct Rect {
    int x, y, width, height;
};

struct Color { unsigned char r, g, b; };

class GUIElement {
public:
    GUIElement(const Rect& rect) : m_rect(rect), visible(true) {}
    virtual ~GUIElement() = default;

    virtual void draw(void* windowHandle = nullptr, void* display = nullptr) = 0;
    virtual void onClick(int x, int y) {}

    bool contains(int x, int y) const {
        return x >= m_rect.x && x <= m_rect.x + m_rect.width &&
            y >= m_rect.y && y <= m_rect.y + m_rect.height;
    }

    void setRect(const Rect& r) { m_rect = r; }
    Rect getRect() const { return m_rect; }

    void setVisible(bool v) { visible = v; }
    bool isVisible() const { return visible; }

protected:
    Rect m_rect;
    bool visible;
};

#endif // GUIELEMENT_H
