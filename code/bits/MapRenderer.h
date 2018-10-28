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

#include <gf/Entity.h>
#include <gf/ResourceManager.h>
#include <gf/Sprite.h>
#include <gf/TileLayer.h>

#include "WorldData.h"
#include "WorldState.h"

namespace akgr {

  enum Plane {
    High,
    Low,
  };

  class MapGroundRenderer : public gf::Entity {
  public:
    MapGroundRenderer(const WorldData& data, const WorldState& state, gf::ResourceManager& resources);

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    const WorldData& m_data;
    const WorldState& m_state;

    std::vector<gf::TileLayer> m_layers;
  };


  class MapTileRenderer : public gf::Entity {
  public:
    MapTileRenderer(Plane plane, const WorldData& data, const WorldState& state, gf::ResourceManager& resources);

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    const WorldData& m_data;
    const WorldState& m_state;

    std::vector<gf::TileLayer> m_layers;
  };


  class MapSpriteRenderer : public gf::Entity {
  public:

    MapSpriteRenderer(Plane plane, const WorldData& data, const WorldState& state, gf::ResourceManager& resources);

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    const WorldData& m_data;
    const WorldState& m_state;

    std::vector<std::vector<gf::Sprite>> m_layers;
  };


}

#endif // AKGR_MAP_RENDERER_H
