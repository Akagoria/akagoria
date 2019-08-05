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
#include "AspectRenderer.h"

#include <gf/Coordinates.h>
#include <gf/Color.h>
#include <gf/RenderTarget.h>
#include <gf/Rect.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Text.h>

namespace akgr {

  namespace {

    constexpr gf::Vector2f AspectSize = { 0.25f, 0.03f };
    constexpr float AspectIconSize = 64.0f;
    constexpr float AspectCharacterSize = 0.0259;

    void drawAspect(gf::RenderTarget& target, const gf::RenderStates& states, AspectValue& aspect, gf::Vector2f relativePosition, const gf::Color4f& color,
        const gf::Texture& texture, const gf::RectF& textureRect, gf::Font& font, const std::string& name) {
      gf::Coordinates coords(target);

      gf::Vector2f position = coords.getRelativePoint(relativePosition);
      gf::Vector2f size = coords.getRelativeSize(AspectSize);
      float thickness = coords.getRelativeSize({ 0.0f, 0.002f }).height;

      {
        gf::RoundedRectangleShape shape(size, thickness);
        shape.setColor(gf::Color::Gray(0.4f) * gf::Color::Opaque(0.7f));
        shape.setOutlineColor(gf::Color::White);
        shape.setOutlineThickness(thickness);
        shape.setPosition(position);
        target.draw(shape, states);
      }

      {
        float percent = static_cast<float>(aspect.value) / static_cast<float>(aspect.max);
        gf::Vector2f bar = size * gf::Vector2f(percent, 1.0f);
        gf::RectangleShape shape(bar);
        shape.setColor(color);
        shape.setPosition(position);
        target.draw(shape, states);
      }

      float insideHeight = 0.8f * size.height;
      float insidePadding = 0.1f * size.height;


      {
        gf::Color4f iconColor = gf::Color::White * gf::Color::Opaque(0.7f); // gf::Color::darker(color);
        gf::Sprite sprite(texture, textureRect);
        sprite.scale(insideHeight / AspectIconSize);
        sprite.setColor(iconColor);
        sprite.setPosition(position + insidePadding);
        target.draw(sprite, states);
      }

      {
        gf::Color4f fontColor = gf::Color::White * gf::Color::Opaque(0.8f); // gf::Color::darker(color);
        std::string str = std::to_string(aspect.value) + '/' + std::to_string(aspect.max) + ' ' + name;
        unsigned characterSize = coords.getRelativeCharacterSize(AspectCharacterSize); // static_cast<unsigned>(insideHeight)
        gf::Text text(str, font, characterSize);
        text.setColor(fontColor);
        text.setOutlineColor(gf::Color::Black);
        text.setOutlineThickness(thickness / 2);
        text.setPosition(position + gf::Vector2f(size.width - insidePadding, size.height / 2));
        text.setAnchor(gf::Anchor::CenterRight);
        target.draw(text, states);
      }


    }

  }

  AspectRenderer::AspectRenderer(WorldState& state, gf::ResourceManager& resources)
  : gf::Entity(50)
  , m_state(state)
  , m_texture(resources.getTexture("pictures/icons.png"))
  , m_font(resources.getFont("fonts/DejaVuSansMono-Bold.ttf"))
  {

  }

  void AspectRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    if (m_state.operation != WorldOperation::Walk) {
      return;
    }

    drawAspect(target, states, m_state.hero.aspect.hp, { 0.015f, 0.015f }, gf::Color4f(0.75f, 0.25f, 0.25f, 1.0f),
        m_texture, gf::RectF({ 0.0f, 0.0f }, { 0.125f, 0.125f }), m_font, "HP");
    drawAspect(target, states, m_state.hero.aspect.mp, { 0.015f, 0.055f }, gf::Color4f(0.25f, 0.25f, 0.75f, 1.0f),
        m_texture, gf::RectF({ 0.125f, 0.0f }, { 0.125f, 0.125f }), m_font, "MP");
    drawAspect(target, states, m_state.hero.aspect.vp, { 0.015f, 0.095f }, gf::Color4f(0.25f, 0.75f, 0.25f, 1.0f),
        m_texture, gf::RectF({ 0.625f, 0.0f }, { 0.125f, 0.125f }), m_font, "VP");
  }

}
