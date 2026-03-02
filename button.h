#ifndef BUTTON_H
#define BUTTON_H

#include <raylib.h>
#include <functional>

using ClickHandler = std::function<void()>;

struct Button {
public:
    Button* text(char*);
    Button* font_size(int);
    Button* bg_color(Color);
    Button* fg_color(Color);
    Button* size(int, int);
    Button* position(int, int);
    Button* on_click(ClickHandler);

    char* text() { return m_text; }
    int font_size() { return m_fontSize; }
    Color bg_color() { return m_bg; }
    Vector2 size() { return (Vector2) {(float)m_minWidth, (float)m_minWidth}; }
    Vector2 position() { return (Vector2) {(float)m_x, (float)m_y}; }

    int* x() { return &m_x; }
    int* y() { return &m_y; }

    void update();
    void draw();

private:
    char* m_text;
    int m_fontSize;
    Color m_bg;
    Color m_fg;
    int m_x;
    int m_y;
    int m_minWidth;
    int m_minHeight;
    ClickHandler m_onClick;
};


#endif // BUTTON_H
