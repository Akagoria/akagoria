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
#ifndef AKGR_LOCATION_TYPES_H
#define AKGR_LOCATION_TYPES_H

#include <gf/Vector.h>
#include <gf/VectorOps.h>

namespace akgr {

  struct Location {
    gf::Vector2f position = { 0.0f, 0.0f };
    int32_t floor = 0;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, Location& data) {
    return ar | data.position | data.floor;
  }

}

#endif // AKGR_LOCATION_TYPES_H
