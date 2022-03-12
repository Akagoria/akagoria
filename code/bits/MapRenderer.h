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
#ifndef AKGR_MAP_RENDERER_H
#define AKGR_MAP_RENDERER_H

#include <vector>

#include <gf/ResourceManager.h>
#include <gf/Sprite.h>
#include <gf/TileLayer.h>

#include "FloorRenderer.h"
#include "WorldData.h"
#include "WorldState.h"
#include "WorldScenery.h"

namespace akgr {

  enum class Plane {
    High,
    Low,
    Ground,
  };

  class MapTileRenderer : public FloorRenderer {
  public:
    MapTileRenderer(Plane plane, const WorldData& data, gf::ResourceManager& resources);

    void renderFloor(gf::RenderTarget& target, const gf::RenderStates& states, int32_t floor) override;

  private:
    const WorldData& m_data;
    std::vector<gf::TileLayer> m_layers;
  };


  class MapSpriteRenderer : public FloorRenderer {
  public:
    MapSpriteRenderer(Plane plane, const WorldData& data, gf::ResourceManager& resources);

    void renderFloor(gf::RenderTarget& target, const gf::RenderStates& states, int32_t floor) override;

  private:
    const WorldData& m_data;
    std::vector<std::vector<gf::Sprite>> m_layers;
  };


}

#endif // AKGR_MAP_RENDERER_H
