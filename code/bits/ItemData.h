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
#ifndef AKGR_ITEM_DATA_H
#define AKGR_ITEM_DATA_H

#include <string>
#include <map>

#include <gf/Id.h>
#include <gf/Vector.h>

#include "Shape.h"

namespace akgr {

  struct ItemResource {
    std::string name;
    gf::Path path;
    gf::Vector<int32_t, 2> size;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, ItemResource& data) {
    return ar | data.name | data.path | data.size;
  }

  struct ItemGraphics {
    gf::Id resource;
    int32_t index;
    float scale;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, ItemGraphics& data) {
    return ar | data.resource | data.index | data.scale;
  }

  enum class ItemType {
    MeleeWeapon,
    Armor,
    RangedWeapon,
    Explosive,
    Potion,
    Recipes,
    Ingredient,
    Manuscript, // books and parchments
    Rare, // quest objects, rare objects
  };

  struct ItemData {
    std::string name;
    std::string description;
    Shape shape;
    ItemGraphics graphics;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, ItemData& data) {
    return ar | data.name | data.description | data.shape | data.graphics;
  }

  struct ItemCatalogueData {
    std::map<gf::Id, ItemResource> resources;
    std::map<gf::Id, ItemData> items;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, ItemCatalogueData& data) {
    return ar | data.resources | data.items;
  }

}

#endif // AKGR_ITEM_DATA_H
