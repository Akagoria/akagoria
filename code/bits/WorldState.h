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
#ifndef AKGR_WORLD_STATE_H
#define AKGR_WORLD_STATE_H

#include <vector>

#include <gf/Path.h>

#include "CharacterState.h"
#include "DialogState.h"
#include "HeroState.h"
#include "NotificationState.h"
#include "PhysicsState.h"

namespace akgr {

  struct WorldData;

  struct WorldState {
    PhysicsState physics;
    std::vector<NotificationState> notifications;
    std::vector<CharacterState> characters;
    HeroState hero;

    bool loadFromFile(const gf::Path& filename);
    bool saveToFile(const gf::Path& filename);

    void bind(const WorldData& data);
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, WorldState& state) {
    return ar /* | state.physics */ | state.notifications | state.characters | state.hero;
  }

}

#endif // AKGR_WORLD_STATE_H
