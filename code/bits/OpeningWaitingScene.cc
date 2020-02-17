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
#include "OpeningWaitingScene.h"

#include <cstdlib>

#include <gf/Unused.h>

#include "Akagoria.h"
#include "OpeningAct.h"

namespace akgr {

  OpeningWaitingScene::OpeningWaitingScene(Akagoria& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_waiting(game.root.data, game.resources)
  {
    addHudEntity(m_waiting);
  }

  void OpeningWaitingScene::doUpdate(gf::Time time) {
    gf::unused(time);

    if (m_game.opening.loading.valid() && m_game.opening.loading.wait_for(std::chrono::seconds::zero()) == std::future_status::ready) {

      if (!m_game.opening.loading.get()) {
        std::exit(EXIT_FAILURE);
      }

      m_game.popAllScenes(); // TODO
    }
  }

}
