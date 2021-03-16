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
#ifndef AKGR_DATA_REF_H
#define AKGR_DATA_REF_H

#include <cassert>
#include <cinttypes>
#include <map>
#include <memory>

#include <gf/Id.h>
#include <gf/Log.h>

namespace akgr {

  template<typename T>
  struct DataRef {
    gf::Id id = gf::InvalidId;
    const T *data = nullptr;

    void bind(const std::map<gf::Id, T>& database) {
      auto it = database.find(id);

      if (it == database.end()) {
        gf::Log::error("Could not bind reference %" PRIX64 "\n", id);
        data = nullptr;
        return;
      }

      data = std::addressof(it->second);
    }

    operator bool() const noexcept {
      return id != gf::InvalidId;
    }

    const T& operator()() const {
      assert(data != nullptr);
      return *data;
    }

  };

  template<typename Archive, typename T>
  Archive& operator|(Archive& ar, DataRef<T>& ref) {
    return ar | ref.id;
  }

}

#endif // AKGR_DATA_REF_H
