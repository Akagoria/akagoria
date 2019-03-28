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
#ifndef AKGR_WORLD_DATA_H
#define AKGR_WORLD_DATA_H

#include <map>

#include <gf/Id.h>
#include <gf/Path.h>

#include "AreaData.h"
#include "CharacterData.h"
#include "DialogData.h"
#include "ItemData.h"
#include "LocationData.h"
#include "MapData.h"
#include "NotificationData.h"
#include "PhysicsData.h"
#include "ShrineData.h"

namespace akgr {

  struct WorldData {
    MapData map;
    PhysicsData physics;
    std::map<gf::Id, AreaData> areas;
    std::map<gf::Id, LocationData> locations;
    std::vector<ShrineData> shrines;

    std::map<gf::Id, DialogData> dialogs;
    std::map<gf::Id, NotificationData> notifications;
    std::map<gf::Id, CharacterData> characters;
    ItemCatalogueData catalogue;

    bool loadFromFile(const gf::Path& filename);
    bool saveToFile(const gf::Path& filename);
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, WorldData& data) {
    return ar | data.map | data.physics | data.areas | data.locations | data.shrines
        | data.dialogs | data.notifications | data.characters | data.catalogue;
  }

}

#endif // AKGR_WORLD_DATA_H
