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
#ifndef AKGR_WORLD_BASE_SCENE_H
#define AKGR_WORLD_BASE_SCENE_H

#include <gf/Scene.h>

#include <gfb2d/PhysicsDebugger.h>

#include "CommandsHelperRenderer.h"
#include "UniverseRenderer.h"
#include "PhysicsRuntime.h"

namespace akgr {
  struct Akagoria;

  class WorldBaseScene : public gf::Scene {
  public:
    WorldBaseScene(Akagoria& game);

  private:
    void doHandleActions(gf::Window& window) override;
    void doUpdate(gf::Time time) override;

  private:
    Akagoria& m_game;

    UniverseRenderer m_universe;
    CommandsHelperRenderer m_helper;

    gfb2d::PhysicsDebugger m_debug;
    PhysicsListener m_listener;
  };

}

#endif // AKGR_WORLD_BASE_SCENE_H
