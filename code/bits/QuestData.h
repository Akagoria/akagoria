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
#ifndef AKGR_QUEST_DATA_H
#define AKGR_QUEST_DATA_H

#include <string>

namespace akgr {

  enum class QuestType {
    Short,
    Medium,
    Long,
  };

  struct QuestData {
    std::string name;
    QuestType type;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, QuestData& data) {
    return ar | data.name | data.type;
  }

}

#endif // AKGR_QUEST_DATA_H
