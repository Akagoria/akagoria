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
#ifndef AKGR_HERO_STATE_H
#define AKGR_HERO_STATE_H

#include <set>

#include <gf/Id.h>
#include <gf/Move.h>

#include "AspectState.h"
#include "AttributeState.h"
#include "DialogState.h"
#include "InventoryState.h"
#include "PhysicsState.h"
#include "ProgressionState.h"

namespace akgr {

  struct HeroMove {
    gf::LinearMove linear = gf::LinearMove::None;
    gf::AngularMove angular = gf::AngularMove::None;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, HeroMove& move) {
    return ar | move.linear | move.angular;
  }

  struct HeroState {
    HeroMove move;

    std::set<gf::Id> requirements;
    AspectState aspect;
    AttributeState attributes;
    ProgressionState progression;

    InventoryState inventory;

    DialogState dialog;
    PhysicsBody physics;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, HeroState& state) {
    return ar | state.move | state.requirements | state.aspect | state.attributes | state.progression
        | state.inventory | state.dialog | state.physics;
  }

}

#endif // AKGR_HERO_STATE_H
