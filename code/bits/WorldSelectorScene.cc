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
#include "WorldSelectorScene.h"

#include "Akagoria.h"
#include "WorldAct.h"

namespace akgr {

  WorldSelectorScene::WorldSelectorScene(Akagoria& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_selector(game.root.data, game.root.scenery, game.slots, game.theme)
  {
    addHudEntity(m_selector);

    addAction(m_game.commands.menuUp);
    addAction(m_game.commands.menuDown);
    addAction(m_game.commands.gameUse);
  }

  void WorldSelectorScene::doHandleActions([[maybe_unused]] gf::Window& window) {
    auto& hero = m_game.world.state.hero;

    m_game.root.scenery.helper.status = HelperStatus::Menu;

    hero.move.angular = gf::AngularMove::None;
    hero.move.linear = gf::LinearMove::None;

    if (m_game.commands.menuDown.isActive()) {
      m_game.root.scenery.selector.index.computeNextChoice();
    } else if (m_game.commands.menuUp.isActive()) {
      m_game.root.scenery.selector.index.computePrevChoice();
    }

    if (m_game.commands.gameUse.isActive()) {
      if (m_game.root.scenery.selector.index.choice == SlotSelectorScenery::Back) {
        m_game.root.scenery.selector.index.choice = 0;
        m_game.replaceScene(m_game.worldAct->travel);
      } else {
        // TODO: make it async?
        gf::Log::info("Game saving...\n");
        gf::Clock savingClock;

        assert(m_game.root.scenery.selector.index.choice < SlotManager::SlotCount);
        m_game.slots.saveInSlot(m_game.world.data, m_game.world.state, m_game.root.scenery.selector.index.choice);

        auto savingTime = savingClock.getElapsedTime();
        gf::Log::info("Game saved in %d ms\n", savingTime.asMilliseconds());

        m_game.replaceScene(m_game.worldAct->travel);
      }
    }

  }

}
