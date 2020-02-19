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
#ifndef AKGR_WORLD_HUD_SCENE_H
#define AKGR_WORLD_HUD_SCENE_H

#include <gf/Scene.h>

#include "NotificationRenderer.h"
#include "AspectRenderer.h"
#include "MiniMapRenderer.h"
#include "AreaRenderer.h"

namespace akgr {
  struct Akagoria;

  class WorldHudScene : public gf::Scene {
  public:
    WorldHudScene(Akagoria& game);

  private:
    void doUpdate(gf::Time time) override;

  private:
    Akagoria& m_game;
    AreaRenderer m_area;
    AspectRenderer m_aspect;
    MiniMapRenderer m_map;
    NotificationRenderer m_notifications;

//     DialogRenderer dialog(worldState, theme);
//     SlotSelectorRenderer worldSlotSelector(rootData, rootScenery, theme);

//     GameMenuRenderer gameMenu(rootData, worldState, worldScenery, theme);
//     InventoryRenderer inventory(rootData, worldData, worldState, worldScenery, theme, resources);
//     OptionsRenderer gameOptions(rootData, rootScenery, theme);

  };

}

#endif // AKGR_WORLD_HUD_SCENE_H
