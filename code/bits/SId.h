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
#ifndef AKGR_SID_H
#define AKGR_SID_H

#include <string>

#include <gf/Id.h>

namespace akgr {

  struct SId {
    std::string tag;
    gf::Id id;

    SId& operator=(const std::string& other) {
      tag = other;
      id = gf::hash(tag);
      return *this;
    }
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, SId& data) {
    ar | data.tag;

    if (data.tag.empty()) {
      data.id = gf::InvalidId;
    } else {
      data.id = gf::hash(data.tag);
    }

    return ar;
  }

}

#endif // AKGR_SID_H
