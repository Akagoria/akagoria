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
#include "OpeningMenuScene.h"

#include "Akagoria.h"
#include "OpeningAct.h"

namespace akgr {

  OpeningMenuScene::OpeningMenuScene(Akagoria& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_menu(game.root.data, game.opening.scenery, game.theme)
  {
    addHudEntity(m_menu);

    addAction(m_game.commands.menuUp);
    addAction(m_game.commands.menuDown);
    addAction(m_game.commands.gameUse);
  }

  void OpeningMenuScene::doHandleActions([[maybe_unused]]gf::Window& window) {
    if (m_game.commands.menuDown.isActive()) {
      m_game.opening.scenery.menu.index.computeNextChoice();
    } else if (m_game.commands.menuUp.isActive()) {
      m_game.opening.scenery.menu.index.computePrevChoice();
    }

    if (m_game.commands.gameUse.isActive()) {
      switch (m_game.opening.scenery.menu.getChoice()) {
        case StartMenuScenery::Choice::StartAdventure:
          m_game.opening.loading = std::async(std::launch::async, &Akagoria::loadWorld, &m_game, AdventureChoice::New);
          m_game.replaceScene(m_game.openingAct->waiting);
          break;

        case StartMenuScenery::Choice::LoadAdventure:
          m_game.replaceScene(m_game.openingAct->selector);
          break;

        case StartMenuScenery::Choice::Options:
          m_game.replaceScene(m_game.openingAct->options);
          break;

        case StartMenuScenery::Choice::Quit:
          m_game.popAllScenes();
          break;
      }
    }
  }

}
