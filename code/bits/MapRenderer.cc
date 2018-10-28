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
        case Plane::Low:
          return -5;
        case Plane::High:
          return +15;
      }

      assert(false);
      return 0;
    }

    int getPriorityFromPlaneForTiles(Plane plane) {
      switch (plane) {
        case Plane::Low:
          return -10;
        case Plane::High:
          return +10;
      }

      assert(false);
      return 0;
    }
  }

  /*
   * MapGroundRenderer
   */

  MapGroundRenderer::MapGroundRenderer(const WorldData& data, const WorldState& state, gf::ResourceManager& resources)
  : gf::Entity(-15)
  , m_data(data)
  , m_state(state)
  {
    for (auto& floor : data.map.floors) {
      const TextureLayer& textureLayer = floor.groundTiles;

      if (textureLayer.tiles.isEmpty()) {
        m_layers.push_back(gf::TileLayer({ 0u, 0u }));
      } else {
        assert(data.map.mapSize == textureLayer.tiles.getSize());
        gf::TileLayer layer(data.map.mapSize);
        layer.setTileSize(data.map.tileSize);

        for (auto pos : textureLayer.tiles.getPositionRange()) {
          layer.setTile(pos, textureLayer.tiles(pos));
        }

        const Tileset& tileset = data.map.tilesets[textureLayer.tilesetId];

        layer.setMargin(tileset.margin);
        layer.setSpacing(tileset.spacing);

        gf::Texture& texture = resources.getTexture(tileset.path);
        texture.setSmooth(false);
        texture.generateMipmap();
        layer.setTexture(texture);

        m_layers.push_back(std::move(layer));
      }
    }
  }

  void MapGroundRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    int32_t index = m_state.hero.physics.location.floor - m_data.map.floorMin;

    if (!(0 <= index && static_cast<std::size_t>(index) <= m_layers.size())) {
      gf::Log::debug("Floor: %" PRIi32 ", FloorMin: %" PRIi32 " Index: %" PRIi32 "\n", m_state.hero.physics.location.floor, m_data.map.floorMin, index);
    }

    assert(0 <= index && static_cast<std::size_t>(index) <= m_layers.size());
    target.draw(m_layers[index], states);
  }


  /*
   * MapTileRenderer
   */

  MapTileRenderer::MapTileRenderer(Plane plane, const WorldData& data, const WorldState& state, gf::ResourceManager& resources)
  : gf::Entity(getPriorityFromPlaneForTiles(plane))
  , m_data(data)
  , m_state(state)
  {
    for (auto& floor : data.map.floors) {
      const TextureLayer& textureLayer = plane == Plane::Low ? floor.lowTiles : floor.highTiles;

      if (textureLayer.tiles.isEmpty()) {
        m_layers.push_back(gf::TileLayer({ 0u, 0u }));
      } else {
        assert(data.map.mapSize == textureLayer.tiles.getSize());
        gf::TileLayer layer(data.map.mapSize);
        layer.setTileSize(data.map.tileSize);

        for (auto pos : textureLayer.tiles.getPositionRange()) {
          layer.setTile(pos, textureLayer.tiles(pos));
        }

        const Tileset& tileset = data.map.tilesets[textureLayer.tilesetId];

        layer.setMargin(tileset.margin);
        layer.setSpacing(tileset.spacing);

        gf::Texture& texture = resources.getTexture(tileset.path);
        texture.setSmooth();
        layer.setTexture(texture);

        m_layers.push_back(std::move(layer));
      }
    }
  }

  void MapTileRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    int32_t index = m_state.hero.physics.location.floor - m_data.map.floorMin;
    assert(0 <= index && static_cast<std::size_t>(index) <= m_layers.size());
    target.draw(m_layers[index], states);
  }


  /*
   * MapSpriteRenderer
   */

  MapSpriteRenderer::MapSpriteRenderer(Plane plane, const WorldData& data, const WorldState& state, gf::ResourceManager& resources)
  : gf::Entity(getPriorityFromPlaneForSprites(plane))
  , m_data(data)
  , m_state(state)
  {
    for (auto& floor : data.map.floors) {
      const SpriteLayer& layer = plane == Plane::Low ? floor.lowSprites : floor.highSprites;

      std::vector<gf::Sprite> sprites;

      for (auto& raw : layer.sprites) {
        const akgr::Tileset& tileset = data.map.tilesets[raw.tilesetId];
        const gf::Texture& texture = resources.getTexture(tileset.path);
        gf::RectF textureRect = texture.computeTextureCoords(raw.subTexture);

        gf::Sprite sprite(texture, textureRect);
        sprite.setPosition(raw.position);
        sprite.setRotation(gf::degreesToRadians(raw.rotation));
        sprite.setAnchor(gf::Anchor::BottomLeft); // see http://docs.mapeditor.org/en/stable/reference/tmx-map-format/#object

        sprites.push_back(sprite);
      }

      m_layers.push_back(std::move(sprites));
    }

  }

  void MapSpriteRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    int32_t index = m_state.hero.physics.location.floor - m_data.map.floorMin;
    assert(0 <= index && static_cast<std::size_t>(index) <= m_layers.size());

    auto& sprites = m_layers[index];

    for (auto& sprite : sprites) {
      target.draw(sprite, states);
    }
  }

}
