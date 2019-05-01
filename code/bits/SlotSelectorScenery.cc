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
#include "SlotSelectorScenery.h"

#include <gf/Log.h>
#include <gf/Paths.h>
#include <gf/Serialization.h>
#include <gf/SerializationOps.h>
#include <gf/Streams.h>

#include "Version.h"

namespace akgr {

  namespace {

    gf::Path getMetaPath(gf::Path path) {
      path.replace_extension(".meta");
      return path;
    }

  }

  /*
   * Slot
   */

  void Slot::loadFromFile(const gf::Path& filename) {
    path = filename;
    active = boost::filesystem::exists(path);

    if (active) {
      time = boost::filesystem::last_write_time(path);

      gf::Path metaPath = getMetaPath(path);

      if (boost::filesystem::exists(metaPath)) {
        gf::FileInputStream file(metaPath);
        gf::Deserializer ar(file);
        ar | meta;
      } else {
        meta.area = "<unknown>";
      }
    }
  }

  void Slot::save() {
    gf::Path metaPath = getMetaPath(path);
    gf::FileOutputStream file(metaPath);
    gf::Serializer ar(file, MetaVersion);
    ar | meta;
  }

  /*
   * SlotSelectorScenery
   */

  void SlotSelectorScenery::load() {
    gf::Path saveDirectory = gf::Paths::getPrefPath("akagoria", "kalista");
    gf::Log::info("Save directory: %s\n", saveDirectory.string().c_str());

    for (int i = 0; i < SlotCount; ++i) {
      std::string filename = "slot" + std::to_string(i) + ".dat";
      gf::Path path = saveDirectory / filename;
      games[i].loadFromFile(path);
    }
  }

  Slot& SlotSelectorScenery::getSlot() {
    assert(index.choice < SlotCount);
    return games[index.choice];
  }

}
