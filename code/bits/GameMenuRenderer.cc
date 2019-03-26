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
#include "GameMenuRenderer.h"

#include <gf/VectorOps.h>

#include "Menu.h"

using namespace gf::literals;

namespace akgr {

  GameMenuRenderer::GameMenuRenderer(const UIData& data, const WorldState& state, const WorldScenery& scenery, const Display& display)
  : gf::Entity(10)
  , m_data(data)
  , m_state(state)
  , m_scenery(scenery)
  , m_display(display)
  {

  }

  void GameMenuRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    if (m_state.operation != WorldOperation::Menu) {
      return;
    }

    m_display.renderBox(target, states, { Menu::Position, Menu::TotalSize(GameMenuScenery::ItemCount) });
    m_display.renderString(target, states, { Menu::ItemPosition(GameMenuScenery::Inventory), Menu::ItemSize }, Menu::CharacterSize, m_data.getUIMessage("MenuInventory"_id));
    m_display.renderString(target, states, { Menu::ItemPosition(GameMenuScenery::Quests), Menu::ItemSize }, Menu::CharacterSize, m_data.getUIMessage("MenuQuests"_id));
    m_display.renderString(target, states, { Menu::ItemPosition(GameMenuScenery::Skills), Menu::ItemSize }, Menu::CharacterSize, m_data.getUIMessage("MenuSkills"_id));
    m_display.renderString(target, states, { Menu::ItemPosition(GameMenuScenery::Options), Menu::ItemSize }, Menu::CharacterSize, m_data.getUIMessage("MenuOptions"_id));
    m_display.renderString(target, states, { Menu::ItemPosition(GameMenuScenery::Quit), Menu::ItemSize }, Menu::CharacterSize, m_data.getUIMessage("MenuQuit"_id));

    m_display.renderArrow(target, states, Menu::ArrowPosition(m_scenery.menu.choice));
  }

}
