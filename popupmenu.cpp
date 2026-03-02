
#include <string.h>
#include "popupmenu.h"


PopUpMenu* PopUpMenu::rect(Rectangle rect) {
    this->m_rect = rect;
    return this;
}

void PopUpMenu::update() {

    bool leftClicked = false;
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && !m_visible) {
        m_visible = true;
        auto mouse_pos = GetMousePosition();
        m_rect.x = mouse_pos.x;
        m_rect.y = mouse_pos.y;
    }
    else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        auto mouse_pos = GetMousePosition();
        if (CheckCollisionPointRec(mouse_pos, m_rect) == false) {
            m_visible = false;
        } else {
            leftClicked = true;
        }
    }

    if (!m_visible) return;
    int y = m_rect.y;
    for (auto& item: m_children) {
        auto rect = item.rect();
        rect->x = m_rect.x;
        rect->y = y;
        rect->width = m_rect.width;
        y += rect->height;
        item.update();

        if (item.is_hovered() && leftClicked && *item.on_click() != nullptr) {
            (*item.on_click())();
        }

    }
}

void PopUpMenu::draw() {
    if (!m_visible) return;
    DrawRectangle(m_rect.x + 5, m_rect.y + 5, m_rect.width, m_rect.height + 5, BLACK);
    DrawRectangle(m_rect.x, m_rect.y, m_rect.width, m_rect.height, GetColor(0xaaaaaaff));
    for (auto child: m_children) {
        child.draw();
    }
}

void PopUpMenu::menu_item(char* text, std::function<void()> func) {
    MenuItem menuItem;
    auto menu_item = menuItem.text(text);
    menu_item
        ->color(GetColor(0xaaaaaaff))
        ->rect({m_rect.x, m_rect.height, m_rect.width, 16})
        ->on_click(func);

    int text_width = strlen(text);
    for (auto child: m_children) {
        int current_len = strlen(child.text());
        text_width = current_len > text_width ? current_len : text_width;
    }
    m_children.push_back(menuItem);
    m_rect.height += menuItem.rect()->height;
    m_rect.width = text_width * 10;
    done();
}

void PopUpMenu::separator() {
    MenuItem menuItem;
    auto menu_item = menuItem.text("");
    menu_item
        ->color(BLACK)
        ->rect({m_rect.x, m_rect.height, m_rect.width, 1})
        ->separator()
        ->on_click(nullptr);

    int text_width = strlen(menu_item->text());
    for (auto child: m_children) {
        int current_len = strlen(child.text());
        text_width = current_len > text_width ? current_len : text_width;
    }
    m_children.push_back(menuItem);
    m_rect.height += menuItem.rect()->height + 4;
    m_rect.width = text_width * 10;
    done();
}

void PopUpMenu::done() {
    //auto last = children.end();
    m_rect.height -= 1.5;
}
