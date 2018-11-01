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
#include "UIData.h"

#include <cinttypes>

#include <boost/locale.hpp>

#include <gf/Log.h>
#include <gf/Serialization.h>
#include <gf/SerializationOps.h>
#include <gf/Streams.h>

#include "Version.h"

namespace akgr {

  bool UIData::loadFromFile(const gf::Path& filename) {
    gf::FileInputStream file(filename);
    gf::CompressedInputStream compressed(file);
    gf::Deserializer ar(compressed);

    ar | *this;
    return true;
  }

  bool UIData::saveToFile(const gf::Path& filename) {
    gf::FileOutputStream file(filename);
    gf::CompressedOutputStream compressed(file);
    gf::Serializer ar(compressed, UIVersion);

    ar | *this;
    return true;
  }

  std::string UIData::getUIMessage(gf::Id id) const {
    auto it = messages.find(id);

    if (it == messages.end()) {
      gf::Log::error("Unknown UI message id: %" PRIX64 "\n", id);
      return "";
    }

    return boost::locale::gettext(it->second.message.c_str());
  }

}
