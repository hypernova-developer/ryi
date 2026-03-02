
/*
 * Ryi Image Viewer
 *
 * Author: Gama Sibusiso
 * Date: 02-March-2026
 *
 */

#ifndef POPUPMENU_H
#define POPUPMENU_H
#include "menuitem.h"

/*
 * PopUpMenu UI element.
 * Accessible by right clicking anywhere on the screen. Contains a list of menu items.
 * Menu items are added by calling menu_item on an instance of PopUpMenu.
 */
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
