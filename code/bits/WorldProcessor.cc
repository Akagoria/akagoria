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
#include "WorldProcessor.h"

#include <limits>

#include "WorldConstants.h"

namespace akgr {

  namespace {

    static constexpr gf::Time AreaUpdatePeriod = gf::seconds(1);
    static constexpr gf::Time HPUpdatePeriod = gf::seconds(11);
    static constexpr gf::Time MPUpdatePeriod = gf::seconds(29);
    static constexpr gf::Time VPUpdatePeriod = gf::seconds(29);

  }

  WorldProcessor::WorldProcessor(const WorldData& data, WorldState& state, WorldScenery& scenery, RootScenery& root, Script& script)
  : m_data(data)
  , m_state(state)
  , m_scenery(scenery)
  , m_root(root)
  , m_script(script)
  {

  }

  void WorldProcessor::update(gf::Time time) {
    float dt = time.asSeconds();

    /*
     * state
     */

    auto &hero = m_state.hero;
    const uint16_t floor = hero.physics.location.floor;

    // hero (pre-update)
    {
      static constexpr float Hop = 150.0f;
      static constexpr float Turn = 3.0f;

      float angle = hero.physics.angle;
      float velocity = 0.0f;

      switch (hero.move.angular) {
        case gf::AngularMove::None:
          break;

        case gf::AngularMove::Left:
          angle -= dt * Turn;
          break;

        case gf::AngularMove::Right:
          angle += dt * Turn;
          break;
      }

      switch (hero.move.linear) {
        case gf::LinearMove::None:
          break;

        case gf::LinearMove::Forward:
          velocity = -Hop;
          break;

        case gf::LinearMove::Backward:
          velocity = Hop * 0.5f;
          break;
      }

      angle = std::fmod(angle, 2 * gf::Pi);
      hero.physics.setAngleAndVelocity(angle, velocity);
    }

    // physics

    m_state.physics.update(time);
    m_script.handleDeferedMessages();

    // notifications

    if (!m_state.notifications.empty()) {
      auto& current = m_state.notifications.front();
      current.elapsed += time;

      if (current.elapsed > current.ref.data->duration) {
        m_state.notifications.erase(m_state.notifications.begin());
      }
    }

    // hero (again)

    hero.physics.pullLocation();

    hero.aspect.hp.update(time, HPUpdatePeriod);
    hero.aspect.mp.update(time, MPUpdatePeriod);
    hero.aspect.vp.update(time, VPUpdatePeriod);

    // ...


    /*
     * scenery
     */

    // shrines

    for (auto& shrine : m_scenery.shrines) {
      if (shrine.data->location.floor != floor) {
        continue;
      }

      for (auto& particle : shrine.particles) {
        if (particle.clockwise) {
          particle.theta += particle.velocity * dt;
        } else {
          particle.theta -= particle.velocity * dt;
        }
      }
    }

    // particles

    m_scenery.vfx.update(time);

    // area

    m_scenery.area.period += time;

    if (m_scenery.area.current == nullptr || m_scenery.area.period > AreaUpdatePeriod) {
      m_scenery.area.period -= AreaUpdatePeriod;

      auto distanceToHero = [&hero](const auto& kv) {
        return gf::naturalDistance(hero.physics.location.position, kv.second.position.center);
      };

      auto it = std::min_element(m_data.areas.begin(), m_data.areas.end(), [&](const auto& lhs, const auto& rhs) {
        return distanceToHero(lhs) < distanceToHero(rhs);
      });

      m_scenery.area.current = &it->second;
    }

    /*
     * root
     */

    auto squareDistanceToHero = [&hero](gf::Vector2f other) {
      return gf::squareDistance(hero.physics.location.position, other);
    };

    for (auto& character : m_state.characters) {
      if (character.dialog.id == gf::InvalidId) {
        continue;
      }

      if (character.physics.location.floor != hero.physics.location.floor) {
        continue;
      }

      if (squareDistanceToHero(character.physics.location.position) < gf::square(DialogDistance)) {
        m_root.helper.status = HelperStatus::Talk;
      }
    }

    for (auto& item : m_state.items) {
      if (item.physics.location.floor != hero.physics.location.floor) {
        continue;
      }

      if (squareDistanceToHero(item.physics.location.position) < gf::square(ItemDistance + item.ref.data->shape.getPhysicalSize())) {
        m_root.helper.status = HelperStatus::Pick;
      }
    }

    for (auto& shrine : m_data.shrines) {
      if (shrine.location.floor != hero.physics.location.floor) {
        continue;
      }

      if (squareDistanceToHero(shrine.location.position) < gf::square(ShrineDistance)) {
        m_root.helper.status = HelperStatus::Use;
      }
    }
  }

}
