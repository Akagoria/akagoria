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
#include "MiniMapRenderer.h"

#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>

namespace akgr {

  namespace {

    constexpr gf::Vector2f MiniMapPosition = { 1.0f - 0.015f, 0.015f };
    constexpr float MiniMapSize = 0.25f;

  }

  MiniMapRenderer::MiniMapRenderer(const WorldState& state)
  : m_state(state)
  {

  }

  void MiniMapRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    if (m_state.operation != WorldOperation::Walk) {
      return;
    }

    gf::Coordinates coords(target);

    gf::Vector2f position = coords.getRelativePoint(MiniMapPosition);
    float size = coords.getRelativeSize({ 0.0f, MiniMapSize }).height;

    // placeholder
    gf::RectangleShape shape({ size, size });
    shape.setColor(gf::Color::Black * gf::Color::Opaque(0.8f));
    shape.setPosition(position);
    shape.setAnchor(gf::Anchor::TopRight);
    target.draw(shape, states);
  }


}


