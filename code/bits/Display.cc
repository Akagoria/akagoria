/*
 * Akagoria, the revenge of Kalista
 * a single-player RPG in an open world with a top-down view.
 *
 * Copyright (c) 2013-2018, Julien Bernard
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Display.h"

#include <gf/Coordinates.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Text.h>

namespace akgr {

  namespace {

    constexpr gf::Color4f RpgBlue(0.015625, 0.03125, 0.515625, 0.9f);
    constexpr float DisplayPadding = 0.01f;
    constexpr float DisplayThickness = 0.002f;

    constexpr float DisplayArrowSize = 64.0f;

  } // namespace


  Display::Display(gf::ResourceManager& resources)
  : m_font(resources.getFont("fonts/DejaVuSans.ttf")) // fonts/Philosopher-Regular.ttf
  , m_arrow(resources.getTexture("pictures/icons.png"))
  {
    resources.getTexture("pictures/icons.png").setSmooth();
  }

  void Display::renderArrow(gf::RenderTarget& target, const gf::RenderStates& states, gf::Vector2f position) const {
    gf::Coordinates coords(target);

    float height = coords.getRelativeSize({ 0.0f, 0.03f }).height;

    gf::Sprite sprite(m_arrow, gf::RectF({ 0.25f, 0.0f }, { 0.125f, 0.125f }));
    sprite.setPosition(coords.getRelativePoint(position));
    sprite.setScale(height / DisplayArrowSize);
    sprite.setAnchor(gf::Anchor::Center);
    target.draw(sprite, states);
  }

  void Display::renderBox(gf::RenderTarget& target, const gf::RenderStates& states, const gf::RectF& area) const {
    gf::Coordinates coords(target);

    gf::Vector2f position = coords.getRelativePoint(area.getPosition());
    gf::Vector2f size = coords.getRelativeSize(area.getSize());
    float thickness = coords.getRelativeSize({ 0.0f, DisplayThickness }).height;

    gf::RoundedRectangleShape shape(size, 5.0f);
    shape.setPosition(position);
    shape.setColor(RpgBlue);
    shape.setOutlineColor(gf::Color::White);
    shape.setOutlineThickness(thickness);
    target.draw(shape, states);
  }

  void Display::renderText(gf::RenderTarget& target, const gf::RenderStates& states, const gf::RectF& area, float characterSize, const std::string& content, gf::Alignment alignment) const {
    gf::Coordinates coords(target);

    gf::Vector2f position = coords.getRelativePoint(area.getPosition());
    gf::Vector2f size = coords.getRelativeSize(area.getSize());

    unsigned absoluteCharacterSize = coords.getRelativeCharacterSize(characterSize);

    gf::Text text(content, m_font, absoluteCharacterSize);
    text.setParagraphWidth(size.width);
    text.setAlignment(alignment);

    text.setColor(gf::Color::White);
    text.setOutlineColor(gf::Color::Black);
    text.setOutlineThickness(1);
    text.setPosition(position);
    text.setAnchor(gf::Anchor::TopLeft);
    target.draw(text, states);
  }

  void Display::renderTextBox(gf::RenderTarget& target, const gf::RenderStates& states, const gf::RectF& area, float characterSize, const std::string& content, gf::Alignment alignment) const {
    renderBox(target, states, area.grow(DisplayPadding));
    renderText(target, states, area, characterSize, content, alignment);
  }

}
