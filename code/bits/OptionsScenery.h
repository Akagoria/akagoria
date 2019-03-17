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
#ifndef AKGR_OPTIONS_SCENERY_H
#define AKGR_OPTIONS_SCENERY_H

#include <string>

#include <gf/Path.h>
#include <gf/Time.h>

namespace akgr {

  struct Options {
    enum class Input : uint8_t {
      Keyboard,
      Gamepad,
    };

    Input input = Input::Keyboard;

    enum class Display : uint8_t {
      Fullscreen,
      Window_960x540,
      Window_1024x576,
      Window_1152x648,
      Window_1280x720,
    };

    Display display = Display::Window_1024x576;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, Options& options) {
    return ar | options.input | options.display;
  }

  struct OptionsScenery {
    Options options;

    void load();
    void save();
  };

}

#endif // AKGR_OPTIONS_SCENERY_H
