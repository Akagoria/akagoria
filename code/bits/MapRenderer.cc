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
#include "MapRenderer.h"

#include <cassert>

#include <gf/RenderTarget.h>
#include <gf/VectorOps.h>

namespace akgr {

  namespace {
    int getPriorityFromPlaneForSprites(Plane plane) {
      switch (plane) {
        case Plane::High:
          return +15;
        case Plane::Low:
          return -5;
        case Plane::Ground:
          assert(false);
          break;
      }

      assert(false);
      return 0;
    }

    int getPriorityFromPlaneForTiles(Plane plane) {
      switch (plane) {
        case Plane::High:
          return +10;
        case Plane::Low:
          return -10;
        case Plane::Ground:
          return -15;
      }

      assert(false);
      return 0;
    }
  }

  /*
   * MapTileRenderer
   */

  MapTileRenderer::MapTileRenderer(Plane plane, const WorldData& data, const WorldState& state, MapTileScenery& scenery)
  : gf::Entity(getPriorityFromPlaneForTiles(plane))
  , m_data(data)
  , m_state(state)
  , m_scenery(scenery)
  {

  }

  void MapTileRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    int32_t index = m_state.hero.physics.location.floor - m_data.map.floorMin;

    if (!(0 <= index && static_cast<std::size_t>(index) <= m_scenery.layers.size())) {
      gf::Log::debug("Floor: %" PRIi32 ", FloorMin: %" PRIi32 " Index: %" PRIi32 "\n", m_state.hero.physics.location.floor, m_data.map.floorMin, index);
    }

    assert(0 <= index && static_cast<std::size_t>(index) <= m_scenery.layers.size());
    auto& layer = m_scenery.layers[index];

    target.draw(layer, states);
  }


  /*
   * MapSpriteRenderer
   */

  MapSpriteRenderer::MapSpriteRenderer(Plane plane, const WorldData& data, const WorldState& state, MapSpriteScenery& scenery)
  : gf::Entity(getPriorityFromPlaneForSprites(plane))
  , m_data(data)
  , m_state(state)
  , m_scenery(scenery)
  {

  }

  void MapSpriteRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    int32_t index = m_state.hero.physics.location.floor - m_data.map.floorMin;
    assert(0 <= index && static_cast<std::size_t>(index) <= m_scenery.layers.size());

    auto& sprites = m_scenery.layers[index];

    for (auto& sprite : sprites) {
      assert(sprite.hasTexture());
      target.draw(sprite, states);
    }
  }

}
