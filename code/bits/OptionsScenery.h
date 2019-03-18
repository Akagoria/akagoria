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

  struct OptionsData {
    enum class Input : uint8_t {
      Keyboard_Arrows,
      Keyboard_Keys,    // WASD
      Gamepad,
    };

    Input input = Input::Keyboard_Arrows;

    static Input next(Input input);
    static Input prev(Input input);

    enum class Display : uint8_t {
      Fullscreen,
      Window_960x540,
      Window_1024x576,
      Window_1152x648,
      Window_1280x720,
    };

    Display display = Display::Window_1024x576;

    static Display next(Display display);
    static Display prev(Display display);

  };

  template<typename Archive>
  Archive& operator|(Archive& ar, OptionsData& data) {
    return ar | data.input | data.display;
  }

  struct OptionsScenery {
    OptionsData data;
    int choice = 0;

    static constexpr int Input    = 0;
    static constexpr int Display  = 1;
    static constexpr int Back     = 2;

    static constexpr int ItemCount = 3;

    void computeNextChoice() {
      choice = (choice + 1) % ItemCount;
    }

    void computePrevChoice() {
      choice = (choice - 1 + ItemCount) % ItemCount;
    }

    void load();
    void save();
  };

}

#endif // AKGR_OPTIONS_SCENERY_H
