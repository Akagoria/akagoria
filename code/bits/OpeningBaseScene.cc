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
#include "OpeningBaseScene.h"

#include <gf/Log.h>

#include "Akagoria.h"
#include "Opening.h"
#include "Root.h"

namespace akgr {

  OpeningBaseScene::OpeningBaseScene(Akagoria& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_logo(game.root.data, game.opening.scenery, game.resources)
  , m_helper(game.root.data, game.root.scenery, game.resources)
  {
    addHudEntity(m_logo);
    addHudEntity(m_helper);

    addAction(m_game.commands.windowClose);
    addAction(m_game.commands.windowFullscreen);
  }

  void OpeningBaseScene::doHandleActions(gf::Window& window) {
    m_game.root.scenery.helper.status = HelperStatus::Menu;

    if (m_game.commands.windowFullscreen.isActive()) {
      window.toggleFullscreen();
    }

    if (m_game.commands.windowClose.isActive()) {
      window.close();
    }
  }

}
