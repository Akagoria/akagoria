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
#ifndef AKGR_PHYSICS_DATA_H
#define AKGR_PHYSICS_DATA_H

#include <vector>
#include <set>
#include <string>

#include <gf/Polyline.h>
#include <gf/Polygon.h>
#include <gf/Vector.h>

#include "LocationTypes.h"
#include "Shape.h"

namespace akgr {

  constexpr float PhysicsScale = 0.02f;

  struct Zone {
    std::string name;
    Location location;
    gf::Polygon polygon;
    std::string message;
    std::set<gf::Id> requirements;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, Zone& data) {
    return ar | data.name | data.location | data.polygon | data.message | data.requirements;
  }

  struct Collision {
    std::string name;
    Location location;
    gf::Polyline line;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, Collision& data) {
    return ar | data.name | data.location | data.line;
  }

  struct Thing {
    std::string name;
    Location location;
    gf::Polyline line;
    float angle;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, Thing& data) {
    return ar | data.name | data.location | data.line | data.angle;
  }

  struct PhysicsData {
    std::vector<Zone> zones;
    std::vector<Collision> collisions;
    std::vector<Thing> things;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, PhysicsData& data) {
    return ar | data.zones | data.collisions | data.things;
  }

}

#endif // AKGR_PHYSICS_DATA_H
