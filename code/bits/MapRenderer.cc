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

    gf::TileLayer bindTextureLayer(const MapData& map, const TextureLayer& textureLayer, gf::ResourceManager& resources) {
      if (textureLayer.tiles.isEmpty()) {
        return gf::TileLayer();
      }

      assert(map.mapSize == textureLayer.tiles.getSize());
      gf::TileLayer layer = gf::TileLayer::createOrthogonal(map.mapSize, map.tileSize);

      std::set<std::size_t> seen;

      for (auto pos : textureLayer.tiles.getPositionRange()) {
        auto cell = textureLayer.tiles(pos);

        if (seen.find(cell.tilesetId) == seen.end()) {
          const Tileset& tileset = map.tilesets[cell.tilesetId];
          const gf::Texture& texture = resources.getTexture(tileset.path);
          // texture.setSmooth(false);
          // texture.generateMipmap();

          auto id = layer.createTilesetId();
          assert(id == cell.tilesetId);
          auto& ts = layer.getTileset(id);
          ts.setTileSize(tileset.tileSize);
          ts.setMargin(tileset.margin);
          ts.setSpacing(tileset.spacing);
          ts.setTexture(texture);

          seen.insert(cell.tilesetId);
        }

        layer.setTile(pos, cell.tilesetId, cell.gid, cell.flip);
      }

      return layer;
    }

    std::vector<gf::Sprite> bindSpriteLayer(const MapData& map, const SpriteLayer& layer, gf::ResourceManager& resources) {
      std::vector<gf::Sprite> sprites;

      for (auto& raw : layer.sprites) {
        const Tileset& tileset = map.tilesets[raw.tilesetId];
        const gf::Texture& texture = resources.getTexture(tileset.path);
        gf::RectF textureRect = texture.computeTextureCoords(raw.subTexture);

        gf::Sprite sprite(texture, textureRect);
        sprite.setPosition(raw.position);
        sprite.setRotation(gf::degreesToRadians(raw.rotation));
        sprite.setAnchor(gf::Anchor::BottomLeft); // see http://docs.mapeditor.org/en/stable/reference/tmx-map-format/#object

        sprites.push_back(sprite);
      }

      return sprites;
    }

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

  MapTileRenderer::MapTileRenderer(Plane plane, const WorldData& data, gf::ResourceManager& resources)
  : FloorRenderer(getPriorityFromPlaneForTiles(plane))
  , m_data(data)
  {
    for (auto& floor : data.map.floors) {
      switch (plane) {
        case Plane::High:
          m_layers.push_back(bindTextureLayer(data.map, floor.highTiles, resources));
          break;
        case Plane::Low:
          m_layers.push_back(bindTextureLayer(data.map, floor.lowTiles, resources));
          break;
        case Plane::Ground:
          m_layers.push_back(bindTextureLayer(data.map, floor.groundTiles, resources));
          break;
      }
    }
  }

  void MapTileRenderer::renderFloor(gf::RenderTarget& target, const gf::RenderStates& states, int32_t floor) {
    int32_t index = floor - m_data.map.floorMin;

    if (!(0 <= index && static_cast<std::size_t>(index) <= m_layers.size())) {
      gf::Log::debug("Floor: %" PRIi32 ", FloorMin: %" PRIi32 " Index: %" PRIi32 "\n", floor, m_data.map.floorMin, index);
    }

    assert(0 <= index && static_cast<std::size_t>(index) <= m_layers.size());
    auto& layer = m_layers[index];

    target.draw(layer, states);
  }


  /*
   * MapSpriteRenderer
   */

  MapSpriteRenderer::MapSpriteRenderer(Plane plane, const WorldData& data, gf::ResourceManager& resources)
  : FloorRenderer(getPriorityFromPlaneForSprites(plane))
  , m_data(data)
  {
    for (auto& floor : data.map.floors) {
      switch (plane) {
        case Plane::High:
          m_layers.push_back(bindSpriteLayer(data.map, floor.highSprites, resources));
          break;
        case Plane::Low:
          m_layers.push_back(bindSpriteLayer(data.map, floor.lowSprites, resources));
          break;
        case Plane::Ground:
          assert(false);
          break;
      }
    }
  }

  void MapSpriteRenderer::renderFloor(gf::RenderTarget& target, const gf::RenderStates& states, int32_t floor) {
    int32_t index = floor - m_data.map.floorMin;
    assert(0 <= index && static_cast<std::size_t>(index) <= m_layers.size());

    auto& sprites = m_layers[index];

    for (auto& sprite : sprites) {
      assert(sprite.hasTexture());
      target.draw(sprite, states);
    }
  }

}
