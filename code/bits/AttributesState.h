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
#ifndef AKGR_ATTRIBUTES_STATE_H
#define AKGR_ATTRIBUTES_STATE_H

#include <cstdint>

#include <gf/Time.h>

namespace akgr {

  struct Attribute {
    int32_t current = 75;
    int32_t max = 100;
    gf::Time period = gf::Time::zero();
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, Attribute& attr) {
    return ar | attr.current | attr.max | attr.period;
  }

  struct AttributesState {
    Attribute hp;
    Attribute mp;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, AttributesState& state) {
    return ar | state.hp | state.mp;
  }

}

#endif // AKGR_ATTRIBUTE_STATE_H
