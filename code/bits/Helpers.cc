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
#include "Helpers.h"

#include <cinttypes>

#include <gf/Log.h>

namespace akgr {

  std::string getUIMessage(const std::map<gf::Id, UIData>& data, gf::Id id) {
    auto it = data.find(id);

    if (it == data.end()) {
      gf::Log::error("Unknown id in UIData: %" PRIX64 "\n", id);
      return "";
    }

    return it->second.message; // TODO: add l10n
  }

}
