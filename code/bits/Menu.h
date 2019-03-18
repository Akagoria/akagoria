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
#ifndef AKGR_MENU_H
#define AKGR_MENU_H

#include <gf/Vector.h>
#include <gf/VectorOps.h>

namespace akgr {

  struct Menu {

    static constexpr gf::Vector2f Position = { 0.002f, 0.002f };

    static constexpr float CharacterSize = 0.03f;

    // items

    static constexpr gf::Vector2f ItemRelPosition = { 0.03f, 0.0f }; // first, relative to menu position
    static constexpr gf::Vector2f ItemSize = { 0.30f, CharacterSize * 1.3f };
    static constexpr float ItemSpacing = 0.01f;

    static constexpr gf::Vector2f TotalSize(int itemCount) {
      return gf::Vector2f(ItemRelPosition.x + ItemSize.x, itemCount * ItemSize.y + (itemCount + 1) * ItemSpacing);
    }

    static constexpr gf::Vector2f ItemPosition(int item) {
      gf::Vector2f pos = Position + ItemRelPosition;
      pos.y += ItemSpacing + item * (ItemSpacing + ItemSize.y);
      return pos;
    }

    // arrow

    static constexpr gf::Vector2f ArrowRelPosition = { 0.015f, ItemSize.y / 2 }; // first, relative to menu position

    static constexpr gf::Vector2f ArrowPosition(int choice) {
      gf::Vector2f pos = Position + ArrowRelPosition;
      pos.y += ItemSpacing + choice * (ItemSpacing + ItemSize.y);
      return pos;
    }

  };

}

#endif // AKGR_MENU_H
