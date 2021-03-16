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
#include "WorldMenuScene.h"

#include <gf/Log.h>
#include <gf/Unused.h>

#include "Akagoria.h"
#include "WorldAct.h"

namespace akgr {

  WorldMenuScene::WorldMenuScene(Akagoria& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_menu(game.root.data, game.world.scenery, game.theme)
  {
    addHudEntity(m_menu);

    addAction(m_game.commands.menuUp);
    addAction(m_game.commands.menuDown);
    addAction(m_game.commands.gameUse);
  }

  void WorldMenuScene::doHandleActions(gf::Window& window) {
    gf::unused(window);

    auto& hero = m_game.world.state.hero;

    m_game.root.scenery.helper.status = HelperStatus::Menu;

    hero.move.angular = gf::AngularMove::None;
    hero.move.linear = gf::LinearMove::None;

    if (m_game.commands.menuDown.isActive()) {
      m_game.world.scenery.menu.index.computeNextChoice();
    } else if (m_game.commands.menuUp.isActive()) {
      m_game.world.scenery.menu.index.computePrevChoice();
    }

    if (m_game.commands.gameUse.isActive()) {
      switch (m_game.world.scenery.menu.getChoice()) {
        case GameMenuScenery::Choice::Inventory:
          m_game.world.scenery.inventory.list.updateCount(hero.inventory.items.size());
          m_game.replaceScene(m_game.worldAct->inventory);
          break;

        case GameMenuScenery::Choice::Quests:
          // TODO
          gf::Log::debug("Quests\n");
          break;

        case GameMenuScenery::Choice::Skills:
          // TODO
          gf::Log::debug("Skills\n");
          break;

        case GameMenuScenery::Choice::Options:
          m_game.replaceScene(m_game.worldAct->options);
          break;

        case GameMenuScenery::Choice::BackToAdventure:
          m_game.replaceScene(m_game.worldAct->travel);
          break;

        case GameMenuScenery::Choice::BackToRealLife:
          m_game.popAllScenes();
          break;
      }
    }

  }

}
