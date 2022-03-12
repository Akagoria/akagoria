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
#include "WorldData.h"

#include <gf/Serialization.h>
#include <gf/SerializationOps.h>
#include <gf/Streams.h>
#include <gf/VectorOps.h>

#include "Version.h"

namespace akgr {

  const AreaData *WorldData::getAreaFromPosition(gf::Vector2f position) const {
    auto distanceToHero = [&](const AreaData& area) {
      return gf::naturalDistance(position, area.position.center);
    };

    auto it = std::min_element(areas.begin(), areas.end(), [&](const AreaData& lhs, const AreaData& rhs) {
      return distanceToHero(lhs) < distanceToHero(rhs);
    });

    return std::addressof(*it);
  }

  bool WorldData::loadFromFile(const gf::Path& filename) {
    gf::FileInputStream file(filename);
    gf::CompressedInputStream compressed(file);
    gf::Deserializer ar(compressed);

    ar | *this;
    return true;
  }

  bool WorldData::saveToFile(const gf::Path& filename) {
    gf::FileOutputStream file(filename);
    gf::CompressedOutputStream compressed(file);
    gf::Serializer ar(compressed, DataVersion);

    ar | *this;
    return true;
  }

}

