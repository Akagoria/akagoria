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
#ifndef AKGR_DIALOG_DATA_H
#define AKGR_DIALOG_DATA_H

#include <string>
#include <vector>

#include "SId.h"

namespace akgr {

  struct DialogLine {
    std::string speaker;
    std::string words;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, DialogLine& data) {
    return ar | data.speaker | data.words;
  }

  enum class DialogType : uint8_t {
    Simple  = 1,
    Quest   = 2,
    Story   = 3,
  };

  struct DialogData {
    SId name;
    DialogType type;
    std::vector<DialogLine> content;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, DialogData& data) {
    return ar | data.name | data.type | data.content;
  }

}

#endif // AKGR_DIALOG_DATA_H
