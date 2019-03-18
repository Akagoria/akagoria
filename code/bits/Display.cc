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
#include <gf/Curves.h>
#include <gf/Log.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/StringUtils.h>
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

    gf::RectF bounds = text.getLocalBounds();

    if (bounds.width > size.width || bounds.height > size.height) {
      gf::Log::debug("Display::renderText(): text can not fit in area, area is (%g, %g), bounds is (%g, %g), text is: \"%s\"\n", size.width, size.height, bounds.width, bounds.height, gf::escapeString(content).c_str());
    }

    // DEBUG
//     gf::RectangleShape shape(gf::RectF(position, size));
//     shape.setColor(gf::Color::Transparent);
//     shape.setOutlineColor(gf::Color::Red);
//     shape.setOutlineThickness(1);
//     target.draw(shape, states);
  }

  void Display::renderString(gf::RenderTarget& target, const gf::RenderStates& states, const gf::RectF& area, float characterSize, const std::string& content, gf::Alignment alignment) const {
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
    text.setPosition({ position.x, position.y + size.height / 2 });
    text.setAnchor(gf::Anchor::CenterLeft);
    target.draw(text, states);

    gf::RectF bounds = text.getLocalBounds();

    if (bounds.width > size.width || bounds.height > size.height) {
      gf::Log::debug("Display::renderString(): text can not fit in area, area is (%g, %g), bounds is (%g, %g), text is: \"%s\"\n", size.width, size.height, bounds.width, bounds.height, gf::escapeString(content).c_str());
    }

    // DEBUG
//     gf::RectangleShape shape(gf::RectF(position, size));
//     shape.setColor(gf::Color::Transparent);
//     shape.setOutlineColor(gf::Color::Red);
//     shape.setOutlineThickness(1);
//     target.draw(shape, states);
  }

  void Display::renderChoice(gf::RenderTarget& target, const gf::RenderStates& states, const gf::RectF& area, float characterSize, std::string content) const {
    gf::Coordinates coords(target);

    gf::Vector2f position = coords.getRelativePoint(area.getPosition());
    gf::Vector2f size = coords.getRelativeSize(area.getSize());

    float square = size.height;

    gf::Vector2f textPosition = position;
    textPosition.x += square;
    float width = size.width - 2 * square;

    unsigned absoluteCharacterSize = coords.getRelativeCharacterSize(characterSize);

    gf::Text text(content, m_font, absoluteCharacterSize);
    text.setParagraphWidth(width);
    text.setAlignment(gf::Alignment::Center);
    text.setColor(gf::Color::White);
    text.setOutlineColor(gf::Color::Black);
    text.setOutlineThickness(1);
    text.setPosition({ textPosition.x, textPosition.y + size.height / 2 });
    text.setAnchor(gf::Anchor::CenterLeft);
    target.draw(text, states);

    gf::CompoundCurve back({ square / 2, square / 4 });
    back.lineTo({ square / 4, square / 2 });
    back.lineTo({ square / 2, 3 * square / 4 });
    back.setPosition(position);
    back.setType(gf::Curve::Outlined);
    back.setWidth(size.height / 10);
    back.setColor(gf::Color::White);
    back.setOutlineColor(gf::Color::Black);
    back.setOutlineThickness(1);
    target.draw(back, states);

    gf::CompoundCurve forth({ - square / 2, square / 4 });
    forth.lineTo({ - square / 4, square / 2 });
    forth.lineTo({ - square / 2, 3 * square / 4 });
    forth.setPosition({ position.x + size.width, position.y });
    forth.setType(gf::Curve::Outlined);
    forth.setWidth(size.height / 10);
    forth.setColor(gf::Color::White);
    forth.setOutlineColor(gf::Color::Black);
    forth.setOutlineThickness(1);
    target.draw(forth, states);

    // DEBUG
//     gf::RectangleShape shape(gf::RectF(position, size));
//     shape.setColor(gf::Color::Transparent);
//     shape.setOutlineColor(gf::Color::Red);
//     shape.setOutlineThickness(1);
//     target.draw(shape, states);

  }

  void Display::renderTextBox(gf::RenderTarget& target, const gf::RenderStates& states, const gf::RectF& area, float characterSize, const std::string& content, float padding, gf::Alignment alignment) const {
    renderBox(target, states, area.grow(padding));
    renderText(target, states, area, characterSize, content, alignment);
  }

}
