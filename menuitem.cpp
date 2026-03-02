#include "menuitem.h"

MenuItem* MenuItem::text(char* text) {
    this->m_text = text;
    return this;
}

MenuItem* MenuItem::rect(Rectangle rect) {
    this->m_rect = rect;
    return this;
}

MenuItem* MenuItem::hovered(bool hovered) {
    this->m_hovered = hovered;
    return this;
}

MenuItem* MenuItem::separator() {
    this->m_isSeparator = true;
    return this;
}

MenuItem* MenuItem::color(Color color) {
    this->m_color = color;
    return this;
}

MenuItem* MenuItem::on_click(ClickHandler click) {
    this->m_onClick = click;
    return this;
}

void MenuItem::update() {
    if (m_isSeparator) return;
    auto mouse_pos = GetMousePosition();
    m_hovered = CheckCollisionPointRec(mouse_pos, m_rect);
    static Color prev = m_color;
    m_color = m_hovered ? ORANGE : prev;
}

void MenuItem::draw() {
    if (m_isSeparator) {
        DrawRectangleGradientEx(m_rect, BLACK, BLACK, BLACK, BLACK);
        return;
    }
    DrawRectangleGradientEx(m_rect, m_color, m_color, m_color, m_color);
    DrawText(m_text, m_rect.x + 5, m_rect.y, 12, BLACK);
}
