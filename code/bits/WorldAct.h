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
#ifndef AKGR_WORLD_ACT_H
#define AKGR_WORLD_ACT_H

#include "WorldBaseScene.h"
#include "WorldDialogScene.h"
#include "WorldInventoryScene.h"
#include "WorldMenuScene.h"
#include "WorldOptionsScene.h"
#include "WorldSelectorScene.h"
#include "WorldTravelScene.h"

namespace akgr {
  struct Akagoria;

  struct WorldAct {
    WorldAct(Akagoria& game);

    WorldBaseScene base;
    WorldTravelScene travel;
    WorldDialogScene dialog;
    WorldSelectorScene selector;
    WorldMenuScene menu;
    WorldOptionsScene options;
    WorldInventoryScene inventory;
  };

}

#endif // AKGR_WORLD_ACT_H
