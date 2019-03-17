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
#ifndef AKGR_WORLD_DRIVER_H
#define AKGR_WORLD_DRIVER_H

#include "Commands.h"
#include "RootScenery.h"
#include "Script.h"
#include "WorldData.h"
#include "WorldScenery.h"
#include "WorldState.h"

namespace akgr {

  class WorldDriver {
  public:
    WorldDriver(const WorldData& data, WorldState& state, WorldScenery& scenery, RootScenery& root, const Commands& commands, Script& script);

    void processCommands();

  private:
    const WorldData& m_data;
    WorldState& m_state;
    WorldScenery& m_scenery;
    RootScenery& m_root;
    const Commands& m_commands;
    Script& m_script;
  };

}

#endif // AKGR_WORLD_DRIVER_H
