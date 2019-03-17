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
#include "OptionsScenery.h"

#include <gf/Log.h>
#include <gf/Paths.h>
#include <gf/Serialization.h>
#include <gf/SerializationOps.h>
#include <gf/Streams.h>

#include "Version.h"

namespace akgr {

  void OptionsScenery::load() {
    gf::Path saveDirectory = gf::Paths::getPrefPath("akagoria", "kalista");
    gf::Path optionsPath = saveDirectory / "options.dat";

    if (boost::filesystem::exists(optionsPath)) {
      gf::FileInputStream file(optionsPath);
      gf::Deserializer ar(file);
      ar | options;
    }
  }

  void OptionsScenery::save() {
    gf::Path saveDirectory = gf::Paths::getPrefPath("akagoria", "kalista");
    gf::Path optionsPath = saveDirectory / "options.dat";

    gf::FileOutputStream file(optionsPath);
    gf::Serializer ar(file);
    ar | options;
  }

}
