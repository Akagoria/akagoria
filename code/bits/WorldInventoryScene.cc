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
#include "WorldInventoryScene.h"

#include <gf/Unused.h>

#include "Akagoria.h"
#include "WorldAct.h"

namespace akgr {

  WorldInventoryScene::WorldInventoryScene(Akagoria& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_inventory(game.root.data, game.world.data, game.world.state, game.world.scenery, game.theme, game.resources)
  {
    addHudEntity(m_inventory);

    addAction(m_game.commands.menuUp);
    addAction(m_game.commands.menuDown);
    addAction(m_game.commands.menuPageUp);
    addAction(m_game.commands.menuPageDown);
    addAction(m_game.commands.menuQuit);
    addAction(m_game.commands.gameUse);
  }

  void WorldInventoryScene::doHandleActions(gf::Window& window) {
    gf::unused(window);

    auto& hero = m_game.world.state.hero;

    m_game.root.scenery.helper.status = HelperStatus::Inventory;

    if (m_game.commands.menuDown.isActive()) {
      m_game.world.scenery.inventory.list.computeNextChoice(hero.inventory.items.size());
    } else if (m_game.commands.menuUp.isActive()) {
      m_game.world.scenery.inventory.list.computePrevChoice(hero.inventory.items.size());
    }

    if (m_game.commands.menuPageDown.isActive()) {
      for (std::size_t i = 0; i < m_game.world.scenery.inventory.list.length / 2; ++i) {
        m_game.world.scenery.inventory.list.computeNextChoice(hero.inventory.items.size());
      }
    } else if (m_game.commands.menuPageUp.isActive()) {
      for (std::size_t i = 0; i < m_game.world.scenery.inventory.list.length / 2; ++i) {
        m_game.world.scenery.inventory.list.computePrevChoice(hero.inventory.items.size());
      }
    }

    if (m_game.commands.gameUse.isActive()) {
      // TODO
    }

    if (m_game.commands.menuQuit.isActive()) {
      m_game.replaceScene(m_game.worldAct->menu);
    }

  }

}
