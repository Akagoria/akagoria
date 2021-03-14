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

#include "CharacterRenderer.h"
#include "CommandsHelperRenderer.h"
#include "HeroRenderer.h"
#include "ItemRenderer.h"
#include "MapRenderer.h"
#include "PhysicsRuntime.h"
#include "VfxRenderer.h"

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

    MapTileRenderer m_ground;
    MapTileRenderer m_lowTile;
    MapSpriteRenderer m_lowSprite;
    MapTileRenderer m_highTile;
    MapSpriteRenderer m_highSprite;
    HeroRenderer m_hero;
    CharacterRenderer m_character;
    ItemRenderer m_item;
    VfxRenderer m_vfx;
    CommandsHelperRenderer m_helper;

    gfb2d::PhysicsDebugger m_debug;
    PhysicsListener m_listener;
  };

}

#endif // AKGR_WORLD_BASE_SCENE_H
