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
#ifndef AKGR_INVENTORY_STATE_H
#define AKGR_INVENTORY_STATE_H

#include "DataRef.h"
#include "ItemData.h"

namespace akgr {

  struct InventoryItem {
    DataRef<ItemData> ref;
    int32_t count;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, InventoryItem& item) {
    return ar | item.ref | item.count;
  }

  struct InventoryState {
    std::vector<InventoryItem> items;

    void addItem(DataRef<ItemData> ref) {
      auto it = std::find_if(items.begin(), items.end(), [&](const auto& other) { return ref.id == other.ref.id; });

      if (it == items.end()) {
        items.push_back({ ref, 1 });
      } else {
        ++it->count;
      }
    }
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, InventoryState& state) {
    return ar | state.items;
  }

}

#endif // AKGR_INVENTORY_STATE_H
