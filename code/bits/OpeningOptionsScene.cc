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
#include "OpeningOptionsScene.h"

#include <gf/Log.h>
#include <gf/Unused.h>

#include "Akagoria.h"
#include "OpeningAct.h"

namespace akgr {

  OpeningOptionsScene::OpeningOptionsScene(Akagoria& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_options(game.root.data, game.root.scenery, game.theme)
  {
    addHudEntity(m_options);

    addAction(m_game.commands.menuUp);
    addAction(m_game.commands.menuDown);
    addAction(m_game.commands.menuLeft);
    addAction(m_game.commands.menuRight);
    addAction(m_game.commands.gameUse);
  }

  void OpeningOptionsScene::doHandleActions(gf::Window& window) {
    gf::unused(window);

    if (m_game.commands.menuDown.isActive()) {
      m_game.root.scenery.options.index.computeNextChoice();
    } else if (m_game.commands.menuUp.isActive()) {
      m_game.root.scenery.options.index.computePrevChoice();
    }

    if (m_game.commands.gameUse.isActive()) {
      if (m_game.root.scenery.options.getChoice() == OptionsScenery::Choice::Back) {
        m_game.root.scenery.options.save();
        m_game.replaceScene(m_game.openingAct->menu);
      }
    }

    if (m_game.commands.menuRight.isActive()) {
      m_game.root.scenery.options.computeNextOption();
    }

    if (m_game.commands.menuLeft.isActive()) {
      m_game.root.scenery.options.computePrevOption();
    }
  }

}
