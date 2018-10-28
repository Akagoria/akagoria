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
#ifndef AKGR_SLOT_SELECTOR_SCENERY_H
#define AKGR_SLOT_SELECTOR_SCENERY_H

#include <ctime>
#include <string>

#include <gf/Path.h>
#include <gf/Time.h>

namespace akgr {


  struct SlotMeta {
    std::string area;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, SlotMeta& meta) {
    return ar | meta.area;
  }

  struct Slot {
    bool active = false;
    gf::Path path;
    std::time_t time;
    SlotMeta meta;

    void loadFromFile(const gf::Path& path);
    void save();
  };

  struct SlotSelectorScenery {
    static constexpr int SlotCount = 3;

    void load();
    Slot& getSlot();

    int choice = 0;
    Slot manual[SlotCount];
    Slot quick;

    static constexpr int Back = 4;
    static constexpr int ItemCount = 5;
  };

}

#endif // AKGR_SLOT_SELECTOR_SCENERY_H
