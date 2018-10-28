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
#include "WorldState.h"

#include <cinttypes>
#include <memory>

#include <gf/Log.h>
#include <gf/Serialization.h>
#include <gf/SerializationOps.h>
#include <gf/Streams.h>

#include "Version.h"
#include "WorldData.h"

namespace akgr {

  bool WorldState::loadFromFile(const gf::Path& filename) {
    gf::FileInputStream file(filename);
    gf::CompressedInputStream compressed(file);
    gf::Deserializer ar(compressed);

    ar | *this;
    return true;
  }

  bool WorldState::saveToFile(const gf::Path& filename) {
    gf::FileOutputStream file(filename);
    gf::CompressedOutputStream compressed(file);
    gf::Serializer ar(compressed, StateVersion);

    ar | *this;
    return true;
  }

  void WorldState::bind(const WorldData& data) {
    // physics

    physics.bind(data);

    // notifications

    for (auto& notification : notifications) {
      notification.ref.bind(data.notifications);
      assert(notification.ref.data);
    }

    // characters

    for (auto& character : characters) {
      character.ref.bind(data.characters);
      character.physics.body = physics.createCharacterBody(character.physics.location, character.physics.angle);
      assert(character.ref.data);
    }

    // hero

    if (hero.dialog.ref.id != gf::InvalidId) {
      hero.dialog.ref.bind(data.dialogs);
      assert(hero.dialog.ref.data);
    }

    hero.physics.body = physics.createHeroBody(hero.physics.location, hero.physics.angle);


  }

}
