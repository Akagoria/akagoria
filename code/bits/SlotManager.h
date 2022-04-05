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
#ifndef AKGR_SLOT_MANAGER_H
#define AKGR_SLOT_MANAGER_H

#include <ctime>
#include <string>

#include <gf/Path.h>

namespace akgr {
  struct WorldData;
  struct WorldState;

  struct SlotMeta {
    std::string area;
    std::time_t time;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, SlotMeta& meta) {
    return ar | meta.area | meta.time;
  }

  struct Slot {
    bool active = false;
    gf::Path path;
    SlotMeta meta;

    void loadMetaFromFile(const gf::Path& path);
    void saveMeta();
  };

  struct SlotManager {
    static constexpr std::size_t SlotCount = 5;

    Slot devices[SlotCount];

    void loadSlotMeta();

    void saveInSlot(const WorldData& data, WorldState& state, std::size_t index);
    void saveInLastSlot(const WorldData& data, WorldState& state) {
      saveInSlot(data, state, SlotCount - 1);
    }
  };

}


#endif // AKGR_SLOT_MANAGER_H
