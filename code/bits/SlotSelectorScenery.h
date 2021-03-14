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

#include <string>
#include <filesystem>

#include <gf/Path.h>
#include <gf/Time.h>

#include "ui/Scenery.h"

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
    std::filesystem::file_time_type time;
    SlotMeta meta;

    void loadFromFile(const gf::Path& path);
    void save();
  };

  struct SlotSelectorScenery {
    static constexpr int SlotCount = 5;

    void load();
    Slot& getSlot();

    Slot games[SlotCount];

    ui::WidgetIndexScenery index = { 0, SlotCount + 1 };

    static constexpr int Back = 5;
  };

}

#endif // AKGR_SLOT_SELECTOR_SCENERY_H
