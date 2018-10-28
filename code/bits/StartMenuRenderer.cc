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
#include "StartMenuRenderer.h"

#include <gf/VectorOps.h>

namespace akgr {

  namespace {

    constexpr gf::Vector2f StartMenuPosition(0.002f, 0.002f);

    constexpr float StartMenuCharacterSize = 0.03f;

    constexpr gf::Vector2f StartMenuItemPosition(0.03f, 0.0f); // first, relative to menu position
    constexpr gf::Vector2f StartMenuItemSize(0.22f, 0.04f);
    constexpr float StartMenuItemSpacing = 0.007f;

    constexpr gf::Vector2f StartMenuSize(0.25f, 3 * StartMenuItemSize.y + 4 * StartMenuItemSpacing);

    constexpr gf::Vector2f StartMenuArrowPosition(0.015f, StartMenuItemSize.y / 2); // first, relative to menu position
    constexpr float StartMenuArrowGap = StartMenuItemSpacing + StartMenuItemSize.y;

  }

  StartMenuRenderer::StartMenuRenderer(const OpeningScenery& scenery, const Display& display)
  : gf::Entity(10)
  , m_scenery(scenery)
  , m_display(display)
  {

  }

  void StartMenuRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    if (m_scenery.operation != OpeningOperation::Menu) {
      return;
    }

    m_display.renderBox(target, states, { StartMenuPosition, StartMenuSize });

    gf::Vector2f position = StartMenuPosition + StartMenuItemPosition;
    position.y += StartMenuItemSpacing;

    m_display.renderText(target, states, { position, StartMenuItemSize }, StartMenuCharacterSize, "Start an adventure");

    position.y += StartMenuItemSize.height + StartMenuItemSpacing;

    m_display.renderText(target, states, { position, StartMenuItemSize }, StartMenuCharacterSize, "Load an adventure");

    position.y += StartMenuItemSize.height + StartMenuItemSpacing;

    m_display.renderText(target, states, { position, StartMenuItemSize }, StartMenuCharacterSize, "Quit");

    gf::Vector2f arrowPosition = StartMenuPosition + StartMenuArrowPosition;
    arrowPosition.y += StartMenuArrowGap * m_scenery.menu.choice;

    m_display.renderArrow(target, states, arrowPosition);
  }

}
