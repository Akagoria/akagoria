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
#include "WorldScenery.h"

#include <iostream>
#include <set>

#include "WorldData.h"
#include "WorldState.h"

namespace akgr {

  namespace {

    constexpr std::size_t ShrineParticleCount = 20;
    constexpr float ShrineParticleMinRadius = 30.0f;

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

  }

  void WorldScenery::bind(const WorldData& data, const WorldState& state, gf::ResourceManager& resources, gf::Random& random) {
    // inventory

    inventory.list.updateCount(state.hero.inventory.items.size());

    // map

    for (auto& floor : data.map.floors) {
      map.groundTiles.layers.push_back(bindTextureLayer(data.map, floor.groundTiles, resources));
      map.lowTiles.layers.push_back(bindTextureLayer(data.map, floor.lowTiles, resources));
      map.lowSprites.layers.push_back(bindSpriteLayer(data.map, floor.lowSprites, resources));
      map.highTiles.layers.push_back(bindTextureLayer(data.map, floor.highTiles, resources));
      map.highSprites.layers.push_back(bindSpriteLayer(data.map, floor.highSprites, resources));
    }

    // vfx

    for (auto& shrine : data.landscape.shrines) {
      VfxShrineEmitter emitter;
      emitter.data = &shrine;

      for (std::size_t i = 0; i < ShrineParticleCount; ++i) {
        VfxShrineParticle particle;
        particle.velocity = random.computeUniformFloat(0.5f * gf::Pi, 1.5f * gf::Pi);
        particle.amplitude = ShrineParticleMinRadius;
        particle.theta = random.computeUniformFloat(0.0f, 2 * gf::Pi);
        particle.n = random.computeUniformFloat(1.0f, 3.0f);
        particle.e = random.computeUniformFloat(0.5f, 1.5f);
        particle.clockwise = (i % 2 == 0);
        emitter.particles.push_back(particle);
      }

      vfx.shrineEmitters.push_back(std::move(emitter));
    }

  }



}
