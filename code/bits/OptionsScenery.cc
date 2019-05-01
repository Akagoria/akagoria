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

#include <cassert>

#include <gf/Log.h>
#include <gf/Paths.h>
#include <gf/Serialization.h>
#include <gf/SerializationOps.h>
#include <gf/Streams.h>

#include "Version.h"

namespace akgr {
  /*
   * OptionsData
   */

  OptionsData::Input OptionsData::getInput() const {
    switch (input.choice) {
      case 0:
        return Input::Keyboard;
      case 1:
        return Input::Gamepad;
      default:
        assert(false);
        return Input::Keyboard;
    }
  }

  OptionsData::Display OptionsData::getDisplay() const {
    switch (display.choice) {
      case 0:
        return Display::Fullscreen;
      case 1:
        return Display::Window_960x540;
      case 2:
        return Display::Window_1024x576;
      case 3:
        return Display::Window_1152x648;
      case 4:
        return Display::Window_1280x720;
      default:
        assert(false);
        return Display::Fullscreen;
    }
  }

  /*
   * OptionsScenery
   */

  namespace {

    gf::Path getOptionsPath() {
      return gf::Paths::getPrefPath("akagoria", "kalista") / "options.dat";
    }

  }

  OptionsScenery::Choice OptionsScenery::getChoice() const {
    switch (index.choice) {
      case 0:
        return Choice::Input;
      case 1:
        return Choice::Display;
      case 2:
        return Choice::Back;
      default:
        assert(false);
        return Choice::Back;
    }
  }

  void OptionsScenery::load() {
    gf::Path optionsPath = getOptionsPath();

    if (boost::filesystem::exists(optionsPath)) {
      gf::FileInputStream file(optionsPath);
      gf::Deserializer ar(file);
      ar | data;
    }
  }

  void OptionsScenery::save() {
    gf::Path optionsPath = getOptionsPath();

    gf::FileOutputStream file(optionsPath);
    gf::Serializer ar(file);
    ar | data;
  }

}
