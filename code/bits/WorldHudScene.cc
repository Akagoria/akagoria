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
#include "WorldHudScene.h"

#include <gf/Log.h>

#include "Akagoria.h"
#include "Root.h"
#include "World.h"

namespace akgr {

  namespace {

    constexpr gf::Time AreaUpdatePeriod = gf::seconds(1);

  }

  WorldHudScene::WorldHudScene(Akagoria& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_area(game.resources, game.world.state, game.world.scenery)
  , m_aspect(game.world.state, game.resources)
  , m_map(game.world.state)
  , m_notifications(game.world.state, game.theme)
  {
    addHudEntity(m_area);
    addHudEntity(m_aspect);
    addHudEntity(m_map);
    addHudEntity(m_notifications);
  }

  void WorldHudScene::doUpdate(gf::Time time) {
    auto &hero = m_game.world.state.hero;

    // notifications

    if (!m_game.world.state.notifications.empty()) {
      auto& current = m_game.world.state.notifications.front();
      current.elapsed += time;

      if (current.elapsed > current.ref.data->duration) {
        m_game.world.state.notifications.erase(m_game.world.state.notifications.begin());
      }
    }

    // area

    m_game.world.scenery.area.period += time;

    if (m_game.world.scenery.area.current == nullptr || m_game.world.scenery.area.period > AreaUpdatePeriod) {
      m_game.world.scenery.area.period -= AreaUpdatePeriod;

      auto distanceToHero = [&hero](const auto& kv) {
        return gf::naturalDistance(hero.physics.location.position, kv.second.position.center);
      };

      auto it = std::min_element(m_game.world.data.areas.begin(), m_game.world.data.areas.end(), [&](const auto& lhs, const auto& rhs) {
        return distanceToHero(lhs) < distanceToHero(rhs);
      });

      m_game.world.scenery.area.current = &it->second;
    }
  } // end of doUpdate

}
