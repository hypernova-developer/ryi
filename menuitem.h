#ifndef MENUITEM_H
#define MENUITEM_H

#include <raylib.h>
#include "button.h"

struct MenuItem {
public:

    MenuItem* text(char*);
    MenuItem* rect(Rectangle);
    MenuItem* hovered(bool);
    MenuItem* separator();
    MenuItem* color(Color);
    MenuItem* on_click(ClickHandler click);

    char*      text()      { return m_text; }
    Rectangle*  rect()      { return &m_rect; }
    bool       is_hovered()   { return m_hovered; }
    bool       is_separator() { return m_isSeparator; }
    Color*      color() { return &m_color; }
    ClickHandler* on_click() { return &m_onClick; }

    void update();
    void draw();

private:
    char* m_text;
    bool m_hovered;
    bool m_isSeparator;
    Rectangle m_rect;
    Color m_color;
    ClickHandler m_onClick;
};


#endif // MENUITEM_H
