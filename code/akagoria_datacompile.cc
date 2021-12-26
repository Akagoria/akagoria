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
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cinttypes>
#include <filesystem>
#include <fstream>

#include <gf/Clock.h>
#include <gf/Geometry.h>
#include <gf/Id.h>
#include <gf/Log.h>
#include <gf/Math.h>
#include <gf/Path.h>
#include <gf/StringUtils.h>
#include <gf/Tmx.h>
#include <gf/Transform.h>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp> // for is_any_of

#include <nlohmann/json.hpp>

#include "bits/RootData.h"
#include "bits/WorldData.h"

using namespace gf::literals;

namespace gf {

  void from_json(const nlohmann::json& j, Time& time) {
    int32_t ms;
    j.get_to(ms);
    time = gf::milliseconds(ms);
  }

}

namespace akgr {
  // Value.h

  void from_json(const nlohmann::json& j, Value& value) {
    int32_t raw;
    j.get_to(raw);
    value = Value(raw);
  }

  // SId.h

  void from_json(const nlohmann::json& j, SId& sid) {
    if (j.is_null()) {
      sid.tag = "";
      sid.id = gf::InvalidId;
    } else {
      j.get_to(sid.tag);
      sid.id = gf::hash(sid.tag);
    }
  }

  // Shape.h

  NLOHMANN_JSON_SERIALIZE_ENUM(ShapeType, {
    { ShapeType::None, "none" },
    { ShapeType::Circle, "circle" },
    { ShapeType::Rectangle, "rectangle" },
  })

  void from_json(const nlohmann::json& j, Shape& shape) {
    j.at("type").get_to(shape.type);

    switch (shape.type) {
      case ShapeType::None:
        break;
      case ShapeType::Circle:
        j.at("radius").get_to(shape.circle.radius);
        break;
      case ShapeType::Rectangle:
        j.at("width").get_to(shape.rectangle.width);
        j.at("height").get_to(shape.rectangle.height);
        break;
    }
  }

  // AtlasData.h

  void from_json(const nlohmann::json& j, AtlasData& data) {
    j.at("name").get_to(data.name);
    j.at("path").get_to(data.path);
    j.at("width").get_to(data.size.width);
    j.at("height").get_to(data.size.height);
  }

  void from_json(const nlohmann::json& j, AtlasSprite& data) {
    j.at("atlas").get_to(data.atlas);
    j.at("index").get_to(data.index);
    j.at("scale").get_to(data.scale);
  }

  void from_json(const nlohmann::json& j, AtlasFrame& data) {
    j.at("atlas").get_to(data.atlas);
    j.at("index").get_to(data.index);
    j.at("duration").get_to(data.duration);
  }

  void from_json(const nlohmann::json& j, AtlasAnimation& data) {
    j.at("name").get_to(data.name);
    j.at("frames").get_to(data.frames);
  }

  // CharacterData.h

  void from_json(const nlohmann::json& j, CharacterData& data) {
    j.at("name").get_to(data.name);
    j.at("size").at("width").get_to(data.size.width);
    j.at("size").at("height").get_to(data.size.height);
    j.at("attribute").get_to(data.attribute);
    j.at("level").get_to(data.level);
    j.at("weapon").get_to(data.weapon);
  }

  // Dialog.h

  void from_json(const nlohmann::json& j, DialogLine& data) {
    j.at("speaker").get_to(data.speaker);
    j.at("words").get_to(data.words);
  }

  NLOHMANN_JSON_SERIALIZE_ENUM(DialogType, {
    { DialogType::Simple, "Simple" },
    { DialogType::Quest, "Quest" },
    { DialogType::Story, "Story" },
  })

  void from_json(const nlohmann::json& j, DialogData& data) {
    j.at("name").get_to(data.name);
    j.at("type").get_to(data.type);
    j.at("content").get_to(data.content);
  }

  // HeroData.h

  void from_json(const nlohmann::json& j, HeroData& data) {
    j.at("animations").get_to(data.animations);
    dictSort(data.animations);
  }

  // ItemData.h

  void from_json(const nlohmann::json& j, ItemData& data) {
    j.at("name").get_to(data.name);
    j.at("description").get_to(data.description);
    j.at("shape").get_to(data.shape);
    j.at("sprite").get_to(data.sprite);
  }

  // NotificationData.h

  void from_json(const nlohmann::json& j, NotificationData& data) {
    j.at("name").get_to(data.name);
    j.at("message").get_to(data.message);
    j.at("duration").get_to(data.duration);
  }

  // WeaponData.h

  NLOHMANN_JSON_SERIALIZE_ENUM(WeaponType, {
    { WeaponType::Melee, "melee" },
    { WeaponType::Ranged, "ranged" },
    { WeaponType::Explosive, "explosive" },
    { WeaponType::Elemental, "elemental" },
  })

  void from_json(const nlohmann::json& j, WeaponData& data) {
    j.at("name").get_to(data.name);
    j.at("description").get_to(data.description);
    j.at("type").get_to(data.type);
    j.at("attack").get_to(data.attack);
    j.at("attribute").get_to(data.attribute);
    j.at("aspect").get_to(data.aspect);
    j.at("range").get_to(data.range);
    j.at("angle").get_to(data.angle);
    j.at("warmup").get_to(data.warmup);
    j.at("cooldown").get_to(data.cooldown);
  }

  // UIData.h

  void from_json(const nlohmann::json& j, UIData& data) {
    j.at("name").get_to(data.name);
    j.at("message").get_to(data.message);
  }

}


namespace {

  std::vector<akgr::Collision> computeAutoCollision(const std::vector<gf::SegmentI>& segments, int32_t currentFloor, int& currentCount) {
    auto lines = gf::buildLines(segments);

    std::vector<akgr::Collision> collisions;

    for (auto& line : lines) {
      akgr::Collision collision;
      collision.name = "Auto-collision #" + std::to_string(currentCount++);
      collision.location.floor = currentFloor;
      collision.location.position = gf::Vector2f(0, 0);
      collision.line = std::move(line);
      collisions.push_back(std::move(collision));
    }

    return collisions;
  }


  /*
   * tmx file
   */

  constexpr int32_t InvalidFloor = INT32_MAX;
  constexpr uint32_t InvalidTilesetId = UINT32_MAX;

  gf::Polyline tmxObjectToPolyline(const gf::TmxObject& object) {
    if (object.kind == gf::TmxObject::Polyline) {
      auto& polyline = static_cast<const gf::TmxPolyline&>(object);
      return gf::Polyline(polyline.points, gf::Polyline::Chain);
    }

    if (object.kind == gf::TmxObject::Polygon) {
      auto& polygon = static_cast<const gf::TmxPolygon&>(object);
      return gf::Polyline(polygon.points, gf::Polyline::Loop);
    }

    if (object.kind == gf::TmxObject::Rectangle) {
      auto& rectangle = static_cast<const gf::TmxRectangle&>(object);
      gf::Vector2f size = rectangle.size;

      gf::Polyline polyline(gf::Polyline::Loop);
      polyline.addPoint({ 0.0f,       0.0f });
      polyline.addPoint({ size.width, 0.0f });
      polyline.addPoint({ size.width, size.height });
      polyline.addPoint({ 0.0f,       size.height });

      return polyline;
    }

    if (object.kind == gf::TmxObject::Ellipse) {
      auto& ellipse = static_cast<const gf::TmxEllipse&>(object);

      gf::Vector2f size = ellipse.size;
      gf::Vector2f center = size / 2;

      gf::Polyline polyline(gf::Polyline::Loop);

      static constexpr int EllipseSegmentCount = 10;
      static constexpr float SegmentLength = 50.0f;

      float perimeterApprox = gf::Pi * std::sqrt(2 * (gf::square(size.width) + gf::square(size.height)));
      int count = std::max(EllipseSegmentCount, static_cast<int>(perimeterApprox / SegmentLength));

      for (int i = 0; i < count; ++i) {
        polyline.addPoint(size / 2 * gf::unit(2.0f * gf::Pi * i / count) + center);
      }

      return polyline;
    }

    gf::Log::error("Object kind not supported for polyline: %i\n", static_cast<int>(object.kind));
    return gf::Polyline();
  }

  gf::Polygon tmxObjectToPolygon(const gf::TmxObject& object) {
    if (object.kind == gf::TmxObject::Polygon) {
      auto& polygon = static_cast<const gf::TmxPolygon&>(object);
      return gf::Polygon(polygon.points);
    }

    if (object.kind == gf::TmxObject::Rectangle) {
      auto& rectangle = static_cast<const gf::TmxRectangle&>(object);
      gf::Vector2f size = rectangle.size;

      gf::Polygon polygon;
      polygon.addPoint({ 0.0f,       0.0f });
      polygon.addPoint({ size.width, 0.0f });
      polygon.addPoint({ size.width, size.height });
      polygon.addPoint({ 0.0f,       size.height });

      return polygon;
    }

    gf::Log::error("Object kind not supported for polygon: %i\n", static_cast<int>(object.kind));
    return gf::Polygon();
  }

  struct MapCompiler : public gf::TmxVisitor {
    MapCompiler(akgr::WorldData& d, std::vector<std::string>& s)
    : currentFloor(InvalidFloor)
    , currentTilesetId(0)
    , data(d)
    , strings(s)
    {

    }

    void makeTextureLayer(akgr::TextureLayer& textureLayer, const gf::TmxLayers& map, const gf::TmxTileLayer& tileLayer) {
      assert(textureLayer.name.empty());
      textureLayer.name = tileLayer.name;
      textureLayer.tilesetId = InvalidTilesetId;

      gf::Array2D<int16_t, int32_t> tiles(map.mapSize, -1);

      int k = 0;

      for (auto& cell : tileLayer.cells) {
        int i = k % map.mapSize.width;
        int j = k / map.mapSize.width;
        assert(j < map.mapSize.height);

        int gid = cell.gid;

        if (gid != 0) {
          auto tileset = map.getTileSetFromGID(gid);
          assert(tileset);
          assert(tileset->image);

          gid = gid - tileset->firstGid;
          tiles({ i, j }) = static_cast<int16_t>(gid);

          auto tilesetId = getTilesetId(tileset->image->source, tileset->tileSize, tileset->spacing, tileset->margin);

          if (textureLayer.tilesetId == InvalidTilesetId) {
            assert(tilesetId != InvalidTilesetId);
            textureLayer.tilesetId = tilesetId;
          } else {
            assert(textureLayer.tilesetId == tilesetId);
          }

//           gf::Log::debug("debug: %i (%i,%i) [%i]\n", gid, i, j, tileset->firstGid);

          auto tile = tileset->getTile(gid);
          assert(tile);

          int fenceCount = tile->properties.getIntProperty("fence_count", 0);

          if (fenceCount > 0) {
            gf::Vector2i base(i, j);

            for (int i = 0; i < fenceCount; ++i) {
              auto fence = tile->properties.getStringProperty("fence" + std::to_string(i), "");
              assert(!fence.empty());

              std::vector<std::string> coords;
              boost::algorithm::split(coords, fence, boost::algorithm::is_any_of(","), boost::algorithm::token_compress_on);
              assert(coords.size() == 4);

              gf::SegmentI segment;
              segment.p0 = base * map.tileSize + gf::vec(std::stoi(coords[0]), std::stoi(coords[1]));
              segment.p1 = base * map.tileSize + gf::vec(std::stoi(coords[2]), std::stoi(coords[3]));

              fences.push_back(segment);
            }
          }
        }

        k++;
      }

      if (textureLayer.tilesetId != InvalidTilesetId) {
        textureLayer.tiles = std::move(tiles);
      }
    }

    virtual void visitTileLayer(const gf::TmxLayers& map, const gf::TmxTileLayer& layer) override {
      assert(currentFloor != InvalidFloor);

      std::string kind = layer.properties.getStringProperty("kind", "");

      if (kind.empty()) {
        gf::Log::error("Missing kind in a tile layer: '%s'\n", layer.name.c_str());
        return;
      }

      gf::Log::debug("New layer: '%s' (%s)\n", layer.name.c_str(), kind.c_str());

      if (kind == "ground_tiles") {
        makeTextureLayer(currentFloorData.groundTiles, map, layer);
        return;
      }

      if (kind == "low_tiles") {
        makeTextureLayer(currentFloorData.lowTiles, map, layer);
        return;
      }

      if (kind == "high_tiles") {
        makeTextureLayer(currentFloorData.highTiles, map, layer);
        return;
      }

      gf::Log::error("Unknown kind in a tile layer: '%s'\n", kind.c_str());
    }

    akgr::ShrineType getShrineType(const std::string& type) {
      if (type == "ale") {
        return akgr::ShrineType::Ale;
      }

      if (type == "ike") {
        return akgr::ShrineType::Ike;
      }

      if (type == "moli") {
        return akgr::ShrineType::Moli;
      }

      if (type == "anpa") {
        return akgr::ShrineType::Anpa;
      }

      if (type == "sewi") {
        return akgr::ShrineType::Sewi;
      }

      if (type == "pona") {
        return akgr::ShrineType::Pona;
      }

      if (type == "wawa") {
        return akgr::ShrineType::Wawa;
      }

      if (type == "sijelo") {
        return akgr::ShrineType::Sijelo;
      }

      assert(false);
      return akgr::ShrineType::Pona;
    }

    void makeSpriteLayer(akgr::SpriteLayer& spriteLayer, const gf::TmxLayers& map, const gf::TmxObjectLayer& objectLayer) {
      assert(spriteLayer.name.empty());
      spriteLayer.name = objectLayer.name;

      for (auto& object : objectLayer.objects) {
        assert(object->kind == gf::TmxObject::Tile);

        auto tile = static_cast<gf::TmxTileObject *>(object.get());

        akgr::MapSprite sprite;
        sprite.name = tile->name;

        auto tileset = map.getTileSetFromGID(tile->gid);
        assert(tileset);
        assert(tileset->image);

        // compute texture rect
        auto lid = tile->gid - tileset->firstGid;
        sprite.subTexture = tileset->getSubTexture(lid, tileset->image->size);

        // get texture path
        sprite.tilesetId = getTilesetId(tileset->image->source, tileset->tileSize, tileset->spacing, tileset->margin);

        sprite.position = tile->position;
        sprite.rotation = tile->rotation;

        spriteLayer.sprites.push_back(std::move(sprite));

        /*
         * special kinds
         */

        auto tilesetTile = tileset->getTile(lid);

        if (tilesetTile == nullptr) {
          continue;
        }

        std::string type = tilesetTile->type;

        if (type == "shrine") {
          gf::Vector2f size = sprite.subTexture.getSize();
          gf::Vector2f center = tile->position + size / 2 - gf::diry(size.height);
          gf::Vector2f bottomLeft = tile->position;

          std::string shrineType = tilesetTile->properties.getStringProperty("shrine_type", "");

          akgr::LandscapeShrineData shrine;
          shrine.type = getShrineType(shrineType);
          shrine.location.position = gf::transform(gf::rotation(gf::degreesToRadians(sprite.rotation), bottomLeft), center);
          shrine.location.floor = currentFloor;
          data.landscape.shrines.push_back(std::move(shrine));
        }

        /*
         * physics
         */

        if (!tilesetTile->objects) {
          continue;
        }

        assert(!tilesetTile->objects->objects.empty());
        const gf::TmxObject& shape = *tilesetTile->objects->objects.front();

        gf::Vector2f size = sprite.subTexture.getSize();
        gf::Vector2f center = tile->position + size / 2 - gf::diry(size.height);
        gf::Vector2f bottomLeft = tile->position;

        akgr::Thing thing;
        thing.name = tilesetTile->type;
        thing.location.position = gf::transform(gf::rotation(gf::degreesToRadians(sprite.rotation), bottomLeft), center);
        thing.location.floor = currentFloor;
        thing.angle = sprite.rotation;
        thing.line = tmxObjectToPolyline(shape);

        if (thing.line.getWinding() == gf::Winding::Counterclockwise) {
          thing.line.reverse();
        }

        for (auto& point : thing.line) {
          point += shape.position - size / 2;
        }

        if (thing.name.empty()) {
          gf::Log::warning("Thing with no name at (%g,%g)[%" PRIi32 "]\n", thing.location.position.x, thing.location.position.y, currentFloor);
        }

        data.physics.things.push_back(std::move(thing));
      }
    }

    virtual void visitObjectLayer(const gf::TmxLayers& map, const gf::TmxObjectLayer& layer) override {
      assert(currentFloor != InvalidFloor);

      std::string kind = layer.properties.getStringProperty("kind", "");

      if (kind.empty()) {
        gf::Log::error("Missing kind in an object layer: '%s'\n", layer.name.c_str());
        return;
      }

      if (kind == "zones") {
        for (auto& object : layer.objects) {
          akgr::Zone zone;
          zone.name = object->name;
          zone.location.position = object->position;
          zone.location.floor = currentFloor;
          zone.polygon = tmxObjectToPolygon(*object);

          if (!zone.polygon.isConvex()) {
            gf::Log::error("Polygon not convex in a zone object: '%s'\n", zone.name.c_str());
            continue;
          }

          zone.message = object->properties.getStringProperty("message", "");

          if (zone.message.empty()) {
            gf::Log::error("Missing message in a zone object: '%s'\n", zone.name.c_str());
            continue;
          }

          std::string requirements = object->properties.getStringProperty("requirements", "");
          std::vector<std::string> requirementList;
          boost::algorithm::split(requirementList, requirements, boost::algorithm::is_any_of(","), boost::algorithm::token_compress_on);

          zone.requirements.clear();

          for (auto& requirementString : requirementList) {
            if (requirementString.empty()) {
              continue;
            }

            zone.requirements.insert(gf::hash(requirementString));
          }

          data.physics.zones.push_back(std::move(zone));
        }

        return;
      }

      if (kind == "collisions") {
        for (auto& object : layer.objects) {
          akgr::Collision collision;
          collision.name = object->name;
          collision.location.position = object->position;
          collision.location.floor = currentFloor;
          collision.line = tmxObjectToPolyline(*object);

          if (collision.name.empty()) {
            gf::Log::warning("Collision object with no name at (%g,%g)[%" PRIi32 "]\n", collision.location.position.x, collision.location.position.y, currentFloor);
          }

          data.physics.collisions.push_back(std::move(collision));
        }

        return;
      }

      if (kind == "areas") {
        for (auto& object : layer.objects) {
          if (object->kind != gf::TmxObject::Ellipse && object->kind != gf::TmxObject::Point) {
            gf::Log::error("Wrong kind of objects for areas, must be a point or an ellipse: '%s'\n", object->name.c_str());
            continue;
          }

          akgr::AreaData area;
          area.name = object->name;
          area.position.center = object->position;
          area.position.radius = 0.0f;

          strings.push_back(area.name.tag);

          data.areas.push_back(std::move(area));
        }

        return;
      }

      if (kind == "locations") {
        for (auto& object : layer.objects) {
          if (object->kind != gf::TmxObject::Ellipse && object->kind != gf::TmxObject::Point) {
            gf::Log::error("Wrong kind of objects for locations, must be a point or an ellipse: '%s'\n", object->name.c_str());
            continue;
          }

          akgr::LocationData locationData;
          locationData.name = object->name;
          locationData.location.position = object->position;
          locationData.location.floor = currentFloor;

          data.locations.push_back(std::move(locationData));
        }

        return;
      }

      gf::Log::debug("New layer: '%s' (%s)\n", layer.name.c_str(), kind.c_str());

      if (kind == "low_sprites") {
        makeSpriteLayer(currentFloorData.lowSprites, map, layer);
        return;
      }

      if (kind == "high_sprites") {
        makeSpriteLayer(currentFloorData.highSprites, map, layer);
        return;
      }

      gf::Log::error("Unknown kind in a sprite layer: '%s'\n", kind.c_str());
    }

    virtual void visitGroupLayer(const gf::TmxLayers& map, const gf::TmxGroupLayer& layer) override {
      assert(currentFloor == InvalidFloor);

      currentFloor = layer.properties.getIntProperty("floor", InvalidFloor);
      assert(currentFloor != InvalidFloor);

      gf::Log::debug("New floor: %" PRIi32 "\n", currentFloor);

      auto& floors = data.map.floors;

      if (floors.empty()) {
        assert(data.map.floorMin == InvalidFloor && data.map.floorMax == InvalidFloor);
        data.map.floorMin = data.map.floorMax = currentFloor;
        floors.push_back(akgr::Floor{});
      } else {
        if (currentFloor < data.map.floorMin) {
          floors.insert(floors.begin(), data.map.floorMin - currentFloor, akgr::Floor{});
          data.map.floorMin = currentFloor;
        }

        if (currentFloor > data.map.floorMax) {
          floors.insert(floors.end(), currentFloor - data.map.floorMax, akgr::Floor{});
          data.map.floorMax = currentFloor;
        }
      }

      currentFloorData = { };
      fences.clear();

      for (auto& sublayer : layer.layers) {
        sublayer->accept(map, *this);
      }

      auto collisions = computeAutoCollision(fences, currentFloor, currentAutoCount);
      gf::Log::debug("fences: %zu -- collisions: %zu\n", fences.size(), collisions.size());

      data.physics.collisions.insert(data.physics.collisions.end(), collisions.begin(), collisions.end());

      data.map[currentFloor] = std::move(currentFloorData);
      currentFloor = InvalidFloor;
    }

    int32_t currentFloor;
    akgr::Floor currentFloorData;

    std::vector<gf::SegmentI> fences;
    int currentAutoCount = 0;

    uint32_t currentTilesetId;
    std::map<gf::Path, uint32_t> reverseTilesetId;

    akgr::WorldData& data;
    std::vector<std::string>& strings;


    uint32_t getTilesetId(const gf::Path& path, gf::Vector<int32_t, 2> tileSize, uint32_t spacing, uint32_t margin) {
      auto it = reverseTilesetId.find(path);

      if (it == reverseTilesetId.end()) {
        assert(data.map.tilesets.size() == currentTilesetId);

        akgr::Tileset tileset;
        tileset.path = path;
        tileset.tileSize = tileSize;
        tileset.spacing = spacing;
        tileset.margin = margin;

        data.map.tilesets.push_back(std::move(tileset));

        auto id = currentTilesetId++;
        reverseTilesetId.insert({ path, id });
        return id;
      }

      return it->second;
    }

  };

  void compileTmxMap(const gf::Path& inputDirectory, akgr::WorldData& data, std::vector<std::string>& strings) {

    gf::Path mapFile = inputDirectory / "akagoria.tmx";

    gf::TmxLayers layers;

    if (!layers.loadFromFile(mapFile)) {
      gf::Log::error("Map '%s' does not exist!`\n", mapFile.string().c_str());
      return;
    }

    data.map.floorMin = data.map.floorMax = InvalidFloor;
    MapCompiler compiler(data, strings);
    layers.visitLayers(compiler);

    data.map.mapSize = layers.mapSize;
    data.map.tileSize = layers.tileSize;

    for (auto& tileset : data.map.tilesets) {
      tileset.path = std::filesystem::relative(tileset.path, inputDirectory);
    }

    dictSort(data.areas);
    dictSort(data.locations);
  }

  /*
   * json files
   */

  void compileJsonHero(const gf::Path& filename, akgr::HeroData& data) {
    std::printf("Reading '%s'...\n", filename.string().c_str());
    std::ifstream ifs(filename.string());
    data = nlohmann::json::parse(ifs);
  }

  void compileJsonAtlases(const gf::Path& filename, akgr::Dict<akgr::AtlasData>& data) {
    std::printf("Reading '%s'...\n", filename.string().c_str());
    std::ifstream ifs(filename.string());
    nlohmann::json::parse(ifs).get_to(data);
    dictSort(data);
  }

  void compileJsonDialogs(const gf::Path& filename, akgr::Dict<akgr::DialogData>& data, std::vector<std::string>& strings) {
    std::printf("Reading '%s'...\n", filename.string().c_str());
    std::ifstream ifs(filename.string());
    nlohmann::json::parse(ifs).get_to(data);

    for (auto & dialog : data) {
      for (auto & line : dialog.content) {
        strings.push_back(line.words);
      }
    }

    dictSort(data);
  }

  void compileJsonNotifications(const gf::Path& filename, akgr::Dict<akgr::NotificationData>& data, std::vector<std::string>& strings) {
    std::printf("Reading '%s'...\n", filename.string().c_str());
    std::ifstream ifs(filename.string());
    nlohmann::json::parse(ifs).get_to(data);

    for (auto & notification : data) {
      strings.push_back(notification.message);
    }

    dictSort(data);
  }

  void compileJsonCharacters(const gf::Path& filename, akgr::Dict<akgr::CharacterData>& data) {
    std::printf("Reading '%s'...\n", filename.string().c_str());
    std::ifstream ifs(filename.string());
    nlohmann::json::parse(ifs).get_to(data);
    dictSort(data);
  }

  void compileJsonItems(const gf::Path& filename, akgr::Dict<akgr::ItemData>& data, std::vector<std::string>& strings) {
    std::printf("Reading '%s'...\n", filename.string().c_str());
    std::ifstream ifs(filename.string());
    nlohmann::json::parse(ifs).get_to(data);

    for (auto & item : data) {
      strings.push_back(item.description);
    }

    dictSort(data);
  }

  void compileJsonWeapons(const gf::Path& filename, akgr::Dict<akgr::WeaponData>& data, std::vector<std::string>& strings) {
    std::printf("Reading '%s'...\n", filename.string().c_str());
    std::ifstream ifs(filename.string());
    nlohmann::json::parse(ifs).get_to(data);

    for (auto & weapon : data) {
      strings.push_back(weapon.description);
    }

    dictSort(data);
  }

  void compileJsonUI(const gf::Path& filename, akgr::Dict<akgr::UIData>& data, std::vector<std::string>& strings) {
    std::printf("Reading '%s'...\n", filename.string().c_str());
    std::ifstream ifs(filename.string());
    nlohmann::json::parse(ifs).get_to(data);

    for (auto & ui : data) {
      strings.push_back(ui.message);
    }

    dictSort(data);
  }

  void postProcessAreas(akgr::Dict<akgr::AreaData>& map) {
    for (auto & currentArea : map) {
      gf::Id currentId = currentArea.name.id;

      auto min = std::min_element(map.begin(), map.end(), [currentId, &currentArea](const auto& lhs, const auto& rhs) {
        if (lhs.name.id == currentId) {
          return false;
        }

        if (rhs.name.id == currentId) {
          return true;
        }

        return gf::squareDistance(currentArea.position.center, lhs.position.center) < gf::squareDistance(currentArea.position.center, rhs.position.center);
      });

      currentArea.position.radius = gf::squareDistance(currentArea.position.center, min->position.center) / 2;
    }
  }

  void postProcessCollisions(akgr::PhysicsData& physics, const akgr::Dict<akgr::LocationData>& locations) {
    auto data = dictFind(locations, "Center of the World"_id);

    if (data == nullptr) {
      gf::Log::error("No 'Center of the World' found in the map.\n");
      return;
    }

    gf::Vector2f cotw = data->location.position;

    for (auto& collision : physics.collisions) {
      if (collision.line.isChain()) {
        continue;
      }

      if (collision.line.contains(cotw)) {
        if (collision.line.getWinding() == gf::Winding::Clockwise) {
          collision.line.reverse();
        }
      } else {
        if (collision.line.getWinding() == gf::Winding::Counterclockwise) {
          collision.line.reverse();
        }
      }
    }
  }

  void generateStrings(const std::vector<std::string>& strings, const gf::Path& filename) {
    std::ofstream out(filename.string());

    out << "// generated by akagoria_datacompile, DO NOT EDIT\n";
    out << "// number of strings: " << strings.size() << "\n\n";
    out << "#define N_(S) S\n\n";
    out << "const char *string[] = {\n";

    for (auto& str : strings) {
      out << "\tN_(\"" << gf::escapeString(str) << "\"),\n";
    }

    out << "\tnullptr\n";
    out << "};\n";
  }
}



int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::printf("Usage: akagoria_datacompile <inputDir> <outputDir>\n");
    return EXIT_FAILURE;
  }

  std::printf("Reading akagoria raw data from '%s' and saving to '%s'\n", argv[1], argv[2]);

  gf::Path inputDirectory(argv[1]);

  if (!std::filesystem::is_directory(inputDirectory)) {
    std::fprintf(stderr, "Input directory is not a directory: '%s'\n", argv[1]);
    return EXIT_FAILURE;
  }

  gf::Path outputDirectory(argv[2]);

  if (!std::filesystem::is_directory(outputDirectory)) {
    std::fprintf(stderr, "Output directory is not a directory: '%s'\n", argv[2]);
    return EXIT_FAILURE;
  }

  gf::Clock clock;

  std::vector<std::string> strings;

  // world

  akgr::WorldData worldData;

  compileTmxMap(inputDirectory, worldData, strings);

  gf::Path databaseDirectory = inputDirectory / "database";

  compileJsonHero(databaseDirectory / "hero.json", worldData.hero);
  compileJsonAtlases(databaseDirectory / "atlases.json", worldData.atlases);
  compileJsonDialogs(databaseDirectory / "dialogs.json", worldData.dialogs, strings);
  compileJsonNotifications(databaseDirectory / "notifications.json", worldData.notifications, strings);
  compileJsonCharacters(databaseDirectory / "characters.json", worldData.characters);
  compileJsonItems(databaseDirectory / "items.json", worldData.items, strings);
  compileJsonWeapons(databaseDirectory / "weapons.json", worldData.weapons, strings);

  postProcessAreas(worldData.areas);
  postProcessCollisions(worldData.physics, worldData.locations);

  gf::Path worldOutputFile = outputDirectory / "akagoria.dat";
  worldData.saveToFile(worldOutputFile);


  // root

  akgr::RootData rootData;

  compileJsonUI(databaseDirectory / "ui.json", rootData.ui, strings);

  gf::Path rootOutputFile = outputDirectory / "root.dat";
  rootData.saveToFile(rootOutputFile);


  // strings

  gf::Path stringsOutputFile = outputDirectory / "strings.cc";
  generateStrings(strings, stringsOutputFile);


  // end

  auto duration = clock.getElapsedTime();
  std::printf("Data successfully compiled in %g s\n", duration.asSeconds());

  auto size = std::filesystem::file_size(worldOutputFile);
  double sizeInKib = size / 1024.0;
  double sizeInMib = sizeInKib / 1024.0;
  std::printf("Archive size: %" PRIuMAX " bytes, %.2f KiB, %.2f MiB\n", size, sizeInKib, sizeInMib);

  return 0;
}
