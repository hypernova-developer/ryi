#ifndef POPUPMENU_H
#define POPUPMENU_H
#include "menuitem.h"

struct PopUpMenu {
public:
    PopUpMenu* rect(Rectangle);

    void update();
    void draw();

    void menu_item(char*, ClickHandler func);
    void separator();

private:
    bool m_visible;
    Rectangle m_rect;
    std::vector<MenuItem> m_children;
    void done();
};

#endif // POPUPMENU_H
