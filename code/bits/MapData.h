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
#ifndef AKGR_MAP_DATA_H
#define AKGR_MAP_DATA_H

#include <cassert>
#include <cstdint>
#include <vector>

#include <gf/Array2D.h>
#include <gf/CellTypes.h>
#include <gf/Flags.h>
#include <gf/Path.h>
#include <gf/Rect.h>

namespace akgr {

  /*
   * TextureLayer
   */

  struct TextureCell {
    int16_t gid = -1;
    uint8_t tilesetId = 0;
    gf::Flags<gf::Flip> flip = gf::None;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, TextureCell& data) {
    return ar | data.gid | data.tilesetId | data.flip;
  }

  struct TextureLayer {
    std::string name;
    gf::Array2D<TextureCell, int32_t> tiles;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, TextureLayer& data) {
    return ar | data.name | data.tiles;
  }

  /*
   * Sprite
   */

  struct MapSprite {
    std::string name;
    uint32_t tilesetId;
    gf::Rect<int32_t> subTexture;
    gf::Vector2f position;
    float rotation;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MapSprite& data) {
    return ar | data.name | data.tilesetId | data.subTexture | data.position | data.rotation;
  }

  /*
   * SpriteLayer
   */

  struct SpriteLayer {
    std::string name;
    std::vector<MapSprite> sprites;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, SpriteLayer& data) {
    return ar | data.name | data.sprites;
  }

  /*
   * Floor
   */

  struct Floor {
    TextureLayer groundTiles;
    TextureLayer lowTiles;
    SpriteLayer lowSprites;
    TextureLayer highTiles;
    SpriteLayer highSprites;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, Floor& data) {
    return ar | data.groundTiles | data.lowTiles | data.lowSprites | data.highTiles | data.highSprites;
  }

  /*
   * Tileset
   */

  struct Tileset {
    gf::Path path;
    gf::Vector<int32_t, 2> tileSize;
    int32_t spacing;
    int32_t margin;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, Tileset& data) {
    return ar | data.path | data.tileSize | data.spacing | data.margin;
  }


  /*
   * MapData
   */

  struct MapData {
    int32_t floorMin;
    int32_t floorMax;
    std::vector<Floor> floors;

    gf::Vector<int32_t, 2> mapSize;
    gf::Vector<int32_t, 2> tileSize;
    std::vector<Tileset> tilesets;

    Floor& operator[](int32_t floor) {
      assert(!floors.empty());
      assert(floorMin <= floor && floor <= floorMax);
      assert(floors.size() == static_cast<std::size_t>(floorMax - floorMin + 1));
      return floors[floor - floorMin];
    }

    const Floor& operator[](int32_t floor) const {
      assert(!floors.empty());
      assert(floorMin <= floor && floor <= floorMax);
      assert(floors.size() == static_cast<std::size_t>(floorMax - floorMin + 1));
      return floors[floor - floorMin];
    }

  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MapData& data) {
    return ar | data.floorMin | data.floorMax | data.floors | data.mapSize | data.tileSize | data.tilesets;
  }

}

#endif // AKGR_MAP_DATA_H
