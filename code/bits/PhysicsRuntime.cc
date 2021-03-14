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
#include "PhysicsRuntime.h"

#include "WorldData.h"
#include "WorldState.h"

namespace akgr {

  PhysicsListener::PhysicsListener(Script& script)
  : m_script(script)
  {

    for (auto& zone : script.getData().physics.zones) {
      auto fixture = script.getState().physics.createFixtureForZone(zone);
      m_zones.insert(std::make_pair(fixture, &zone));
    }

  }

  void PhysicsListener::BeginContact(b2Contact* contact) {
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    for (auto fixture : { fixtureA, fixtureB }) {
      auto it = m_zones.find(fixture);

      if (it == m_zones.end()) {
        continue;
      }

      const Zone *zone = it->second;
      auto& requirements = m_script.getState().hero.requirements;

      if (std::includes(requirements.begin(), requirements.end(), zone->requirements.begin(), zone->requirements.end())) {
        m_script.onMessageDefered(zone->message);
      }
    }
  }

}
