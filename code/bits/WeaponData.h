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
#ifndef AKGR_WEAPON_DATA_H
#define AKGR_WEAPON_DATA_H

#include <string>

#include <gf/Id.h>
#include <gf/Time.h>

#include "Value.h"
#include "SId.h"

namespace akgr {

  enum class WeaponType : uint8_t {
    Melee,
    Ranged,
    Explosive,
    Elemental,
  };

  struct WeaponData {
    SId name;
    std::string description;
    WeaponType type;
    Value attack;
    Value attribute; // required attribute
    Value aspect; // cost of aspect (vp or mp)
    float range;
    float angle;
    gf::Time warmup;
    gf::Time cooldown;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, WeaponData& data) {
    return ar | data.name | data.description | data.type | data.attack | data.attribute | data.aspect | data.range | data.angle | data.warmup | data.cooldown;
  }

}

#endif // AKGR_WEAPON_DATA_H
