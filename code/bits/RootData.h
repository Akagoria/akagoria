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
#ifndef AKGR_ROOT_DATA_H
#define AKGR_ROOT_DATA_H

#include <map>
#include <string>

#include <gf/Id.h>
#include <gf/Path.h>

#include "Dict.h"
#include "UIData.h"

namespace akgr {

  struct RootData {
    Dict<UIData> ui;

    bool loadFromFile(const gf::Path& filename);
    bool saveToFile(const gf::Path& filename);

    std::string getUIMessage(gf::Id id) const;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, RootData& data) {
    return ar | data.ui;
  }

}

#endif // AKGR_ROOT_DATA_H
