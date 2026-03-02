#include "button.h"

Button* Button::text(char* text) {
    this->m_text = text;
    return this;
}

Button* Button::font_size(int font_size) {
    this->m_fontSize = font_size;
    return this;
}

Button* Button::bg_color(Color bg) {
    this->m_bg = bg;
    return this;
}
Button* Button::fg_color(Color fg) {
    this->m_fg = fg;
    return this;
}
Button* Button::size(int width, int height) {
    this->m_minWidth = width;
    this->m_minHeight = height;
    return this;
}
Button* Button::position(int x, int y) {
    this->m_x = x;
    this->m_y = y;
    return this;
}

Button* Button::on_click(ClickHandler onclick) {
    this->m_onClick = onclick;
    return this;
}


void Button::draw() {
    auto h = MeasureText(m_text, m_fontSize);
    auto w = TextLength(m_text);
    auto btnW = (float)m_minWidth;
    auto btnH = (float)m_minHeight;
    Rectangle rect = {(float) m_x, (float) m_y, btnW * w, btnH * h / m_fontSize};
    DrawRectanglePro({rect.x + 5, rect.y + 5, rect.width, rect.height}, {0, 0}, 0, BLACK);
    DrawRectanglePro(rect, {0, 0}, 0, m_bg);
    DrawRectangleLinesEx(rect, 1, m_fg);
    DrawText(m_text, rect.x + w + btnW, rect.y + h / m_fontSize, m_fontSize, m_fg);
}

void Button::update() {
    auto h = MeasureText(m_text, m_fontSize);
    auto w = TextLength(m_text);
    auto btnW = (float)m_minWidth;
    auto btnH = (float)m_minHeight;
    Rectangle rect = {(float) m_x, (float) m_y, btnW * w, btnH * h / m_fontSize};
    auto point = GetMousePosition();

    static Color prev = m_fg;
    auto isHovered = CheckCollisionPointRec(point, rect);
    if (isHovered) {
        m_fg = ORANGE;
    } else {
        m_fg = prev;
    }

    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        return;


    if (isHovered && m_onClick) {
        m_onClick();
    }
}

