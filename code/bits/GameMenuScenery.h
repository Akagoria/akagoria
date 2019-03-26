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
#ifndef AKGR_GAME_MENU_SCENERY_H
#define AKGR_GAME_MENU_SCENERY_H

namespace akgr {

  struct GameMenuScenery {
    int choice = 0;

    static constexpr int Inventory  = 0;
    static constexpr int Quests     = 1;
    static constexpr int Skills     = 2;
    static constexpr int Options    = 3;
    static constexpr int Quit       = 4;

    static constexpr int ItemCount = 5;

    void computeNextChoice() {
      choice = (choice + 1) % ItemCount;
    }

    void computePrevChoice() {
      choice = (choice - 1 + ItemCount) % ItemCount;
    }

  };

}

#endif // AKGR_GAME_MENU_SCENERY_H

