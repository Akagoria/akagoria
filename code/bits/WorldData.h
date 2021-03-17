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
#include "AtlasData.h"
#include "CharacterData.h"
#include "DialogData.h"
#include "Dict.h"
#include "ItemData.h"
#include "LandscapeData.h"
#include "LocationData.h"
#include "MapData.h"
#include "NotificationData.h"
#include "PhysicsData.h"
#include "WeaponData.h"

namespace akgr {

  struct WorldData {
    MapData map;
    PhysicsData physics;
    LandscapeData landscape;
    Dict<AreaData> areas;
    Dict<LocationData> locations;

    Dict<AtlasData> atlases;

    Dict<DialogData> dialogs;
    Dict<NotificationData> notifications;
    Dict<CharacterData> characters;
    Dict<ItemData> items;

    Dict<WeaponData> weapons;

    bool loadFromFile(const gf::Path& filename);
    bool saveToFile(const gf::Path& filename);
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, WorldData& data) {
    return ar | data.map | data.physics | data.landscape | data.areas | data.locations
        | data.atlases | data.dialogs | data.notifications | data.characters | data.items | data.weapons;
  }

}

#endif // AKGR_WORLD_DATA_H
