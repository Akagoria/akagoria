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
#include "AreaRenderer.h"

#include <boost/locale.hpp>

#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Text.h>

namespace akgr {

  namespace {

    constexpr gf::Vector2f AreaPositionBase = { 1.0f - 0.015f, 0.015f + 0.01f };
    constexpr float AreaPositionOffset = 0.25f; // same as MiniMapSize
    constexpr float AreaCharacterSize = 0.0259;

  }

  AreaRenderer::AreaRenderer(gf::ResourceManager& resources, const WorldState& state, const WorldScenery& scenery)
  : m_font(resources.getFont("fonts/Philosopher-Regular.ttf"))
  , m_state(state)
  , m_scenery(scenery)
  {

  }

  void AreaRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coords(target);

    gf::Vector2f position = coords.getRelativePoint(AreaPositionBase);
    float offset = coords.getRelativeSize({ 0.0f, AreaPositionOffset }).height;
    position.x -= offset;
    position.y += offset;

    unsigned characterSize = coords.getRelativeCharacterSize(AreaCharacterSize);
    float thickness = coords.getRelativeSize({ 0.0f, 0.001f }).height;

    gf::Text text(boost::locale::gettext(m_scenery.area.current->name.c_str()), m_font, characterSize);
    text.setColor(gf::Color::White);
    text.setOutlineColor(gf::Color::Black);
    text.setOutlineThickness(thickness);
    text.setPosition(position);
    text.setAnchor(gf::Anchor::TopLeft);
    target.draw(text, states);
  }

}
