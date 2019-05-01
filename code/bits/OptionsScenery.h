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

#include "ui/Scenery.h"

namespace akgr {

  struct OptionsData {
    enum class Input : uint8_t {
      Keyboard,
      Gamepad,
    };

    ui::WidgetIndexScenery input = { 0, 2 };

    Input getInput() const;

    enum class Display : uint8_t {
      Fullscreen,
      Window_960x540,
      Window_1024x576,
      Window_1152x648,
      Window_1280x720,
    };

    ui::WidgetIndexScenery display = { 0, 5 };

    Display getDisplay() const;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, OptionsData& data) {
    return ar | data.input.choice | data.display.choice;
  }

  struct OptionsScenery {
    OptionsData data;

    enum class Choice {
      Input,
      Display,
      Back,
    };

    ui::WidgetIndexScenery index = { 0, 3 };

    Choice getChoice() const;

    void computeNextOption() {
      switch (getChoice()) {
        case Choice::Input:
          data.input.computeNextChoice();
          break;
        case Choice::Display:
          data.display.computeNextChoice();
          break;
        default:
          break;
      }
    }

    void computePrevOption() {
      switch (getChoice()) {
        case Choice::Input:
          data.input.computePrevChoice();
          break;
        case Choice::Display:
          data.display.computePrevChoice();
          break;
        default:
          break;
      }
    }

    void load();
    void save();
  };

}

#endif // AKGR_OPTIONS_SCENERY_H
