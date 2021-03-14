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

  gf::Vector2i computeEndPoint(char c, gf::Vector2u tileSize) {
    switch (c) {
      case 'U':
        return gf::Vector2i(tileSize.width / 2, 0);
      case 'R':
        return gf::Vector2i(tileSize.width, tileSize.height / 2);
      case 'D':
        return gf::Vector2i(tileSize.width / 2, tileSize.height);
      case 'L':
        return gf::Vector2i(0, tileSize.height / 2);
      default:
        assert(false);
        break;
    }

    return { 0, 0 };
  }

  /*
   * common
   */

  akgr::ShapeType getShapeType(const std::string& name) {
    if (name == "circle") {
      return akgr::ShapeType::Circle;
    }

    if (name == "rectangle") {
      return akgr::ShapeType::Rectangle;
    }

    gf::Log::error("Unknown shape type: '%s'\n", name.c_str());
    return akgr::ShapeType::None;
  }

  akgr::WeaponType getWeaponType(const std::string& name) {
    if (name == "melee") {
      return akgr::WeaponType::Melee;
    }

    if (name == "ranged") {
      return akgr::WeaponType::Ranged;
    }

    if (name == "explosive") {
      return akgr::WeaponType::Explosive;
    }

    if (name == "elemental") {
      return akgr::WeaponType::Elemental;
    }

    gf::Log::error("Unknown weapon type: '%s'\n", name.c_str());
    return akgr::WeaponType::Melee;
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
            textureLayer.tilesetId = tilesetId;
          } else {
            assert(textureLayer.tilesetId == tilesetId);
          }

          auto tile = tileset->getTile(gid);
          assert(tile);

          int fenceCount = tile->properties.getIntProperty("fence_count", 0);

          if (fenceCount > 0) {
            gf::Vector2i base(i, j);

            for (int i = 0; i < fenceCount; ++i) {
              auto fence = tile->properties.getStringProperty("fence" + std::to_string(i), "");
              assert(!fence.empty());
              assert(fence.size() == 2);

              gf::SegmentI segment;
              segment.p0 = base * map.tileSize + computeEndPoint(fence[0], map.tileSize);
              segment.p1 = base * map.tileSize + computeEndPoint(fence[1], map.tileSize);

              fences.push_back(segment);
            }
          }
        }

        k++;
      }

      textureLayer.tiles = std::move(tiles);
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

      if (type == "pona") {
        return akgr::ShrineType::Pona;
      }

      if (type == "sewi") {
        return akgr::ShrineType::Sewi;
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

          strings.push_back(area.name);

          data.areas.insert({ gf::hash(area.name), std::move(area) });
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

          data.locations.insert({ gf::hash(locationData.name), std::move(locationData) });
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
  }

  /*
   * json files
   */

  void compileJsonAtlases(const gf::Path& filename, std::map<gf::Id, akgr::AtlasData>& data) {
    std::ifstream ifs(filename.string());

    const auto j = nlohmann::json::parse(ifs);

    for (auto kv : j.items()) {
      akgr::AtlasData atlas;
      atlas.name = kv.key();

      auto value = kv.value();
      atlas.path = value["path"].get<std::string>();
      atlas.size.width = value["width"].get<int32_t>();
      atlas.size.height = value["height"].get<int32_t>();

      data.emplace(gf::hash(atlas.name), std::move(atlas));
    }
  }

  akgr::DialogData::Type getDialogType(const std::string& type) {
    if (type == "Simple") {
      return akgr::DialogData::Simple;
    }

    if (type == "Quest") {
      return akgr::DialogData::Quest;
    }

    if (type == "Story") {
      return akgr::DialogData::Story;
    }

    assert(false);
    return akgr::DialogData::Simple;
  }

  void compileJsonDialogs(const gf::Path& filename, std::map<gf::Id, akgr::DialogData>& data, std::vector<std::string>& strings) {
    std::ifstream ifs(filename.string());

    const auto j = nlohmann::json::parse(ifs);

    for (auto kv : j.items()) {
      akgr::DialogData dialog;
      dialog.name = kv.key();

      auto value = kv.value();
      dialog.type = getDialogType(value["type"].get<std::string>());

      for (auto item : value["content"]) {
        akgr::DialogData::Line line;
        line.speaker = item["speaker"].get<std::string>();
        line.words = item["words"].get<std::string>();

        strings.push_back(line.words);

        dialog.content.push_back(std::move(line));
      }

      data.emplace(gf::hash(dialog.name), std::move(dialog));
    }
  }

  void compileJsonNotifications(const gf::Path& filename, std::map<gf::Id, akgr::NotificationData>& data, std::vector<std::string>& strings) {
    std::ifstream ifs(filename.string());

    const auto j = nlohmann::json::parse(ifs);

    for (auto kv : j.items()) {
      akgr::NotificationData notification;
      notification.name = kv.key();

      auto value = kv.value();
      notification.message = value["message"].get<std::string>();
      notification.duration = gf::seconds(value["duration"].get<float>());

      strings.push_back(notification.message);

      data.emplace(gf::hash(notification.name), std::move(notification));
    }
  }

  void compileJsonCharacters(const gf::Path& filename, std::map<gf::Id, akgr::CharacterData>& data) {
    std::ifstream ifs(filename.string());

    const auto j = nlohmann::json::parse(ifs);

    for (auto kv : j.items()) {
      akgr::CharacterData character;
      character.name = kv.key();

      auto value = kv.value();
      character.size.width = value["size"]["width"].get<float>();
      character.size.height = value["size"]["height"].get<float>();

      character.attribute = value["attribute"].get<int32_t>();
      character.level = value["level"].get<int32_t>();

      if (!value["weapon"].is_null()) {
        character.weapon = gf::hash(value["weapon"].get<std::string>());
      } else {
        character.weapon = gf::InvalidId;
      }

      data.emplace(gf::hash(character.name), std::move(character));
    }
  }

  void compileJsonItems(const gf::Path& filename, std::map<gf::Id, akgr::ItemData>& data, std::vector<std::string>& strings) {
    std::ifstream ifs(filename.string());

    const auto j = nlohmann::json::parse(ifs);

    for (auto kv : j.items()) {
      akgr::ItemData item;
      item.name = kv.key();

      auto value = kv.value();
      item.description = value["description"].get<std::string>();
      item.shape.type = getShapeType(value["shape"]["type"].get<std::string>());

      strings.push_back(item.description);

      switch (item.shape.type) {
        case akgr::ShapeType::None:
          break;
        case akgr::ShapeType::Circle:
          item.shape.circle.radius = value["shape"]["radius"].get<float>();
          break;
        case akgr::ShapeType::Rectangle:
          item.shape.rectangle.width = value["shape"]["width"].get<float>();
          item.shape.rectangle.height = value["shape"]["height"].get<float>();
          break;
      }

      item.sprite.atlas = gf::hash(value["sprite"]["atlas"].get<std::string>());
      item.sprite.index = value["sprite"]["index"].get<int>();
      item.sprite.scale = value["sprite"]["scale"].get<float>();

      data.emplace(gf::hash(item.name), std::move(item));
    }

  }

  void compileJsonWeapons(const gf::Path& filename, std::map<gf::Id, akgr::WeaponData>& data, std::vector<std::string>& strings) {
    std::ifstream ifs(filename.string());

    const auto j = nlohmann::json::parse(ifs);

    for (auto kv : j.items()) {
      akgr::WeaponData weapon;
      weapon.name = kv.key();

      auto value = kv.value();
      weapon.description = value["description"].get<std::string>();
      weapon.type = getWeaponType(value["type"].get<std::string>());
      weapon.attack = value["attack"].get<int32_t>();
      weapon.attribute = value["attribute"].get<int32_t>();
      weapon.aspect = value["aspect"].get<int32_t>();
      weapon.range = value["range"].get<float>();
      weapon.angle = value["angle"].get<float>();
      weapon.warmup = gf::milliseconds(value["warmup"].get<int32_t>());
      weapon.cooldown = gf::milliseconds(value["cooldown"].get<int32_t>());

      strings.push_back(weapon.description);

      data.emplace(gf::hash(weapon.name), std::move(weapon));
    }
  }

  void compileJsonUI(const gf::Path& filename, std::map<gf::Id, akgr::UIData>& data, std::vector<std::string>& strings) {
    std::ifstream ifs(filename.string());

    const auto j = nlohmann::json::parse(ifs);

    for (auto kv : j.items()) {
      akgr::UIData ui;
      ui.name = kv.key();

      auto value = kv.value();
      ui.message = value.get<std::string>();

      strings.push_back(ui.message);

      data.emplace(gf::hash(ui.name), std::move(ui));
    }
  }

  void compileJsonPreload(const gf::Path& filename, std::vector<gf::Path>& data) {
    std::ifstream ifs(filename.string());

    const auto j = nlohmann::json::parse(ifs);

    for (auto value : j["preload"]) {
      gf::Path path = value.get<std::string>();
      data.emplace_back(std::move(path));
    }
  }

  void postProcessAreas(std::map<gf::Id, akgr::AreaData>& map) {
    for (auto& kv : map) {
      gf::Id currentId = kv.first;
      akgr::AreaData& currentArea = kv.second;

      auto min = std::min_element(map.begin(), map.end(), [currentId, &currentArea](const auto& lhs, const auto& rhs) {
        if (lhs.first == currentId) {
          return false;
        }

        if (rhs.first == currentId) {
          return true;
        }

        return gf::squareDistance(currentArea.position.center, lhs.second.position.center) < gf::squareDistance(currentArea.position.center, rhs.second.position.center);
      });

      currentArea.position.radius = gf::squareDistance(currentArea.position.center, min->second.position.center) / 2;
    }
  }

  void postProcessCollisions(akgr::PhysicsData& physics, const std::map<gf::Id, akgr::LocationData>& locations) {
    auto it = locations.find("Center of the World"_id);

    if (it == locations.end()) {
      gf::Log::error("No 'Center of the World' found in the map.\n");
      return;
    }

    auto & [ id, data ] = *it;
    gf::Vector2f cotw = data.location.position;

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
  compileJsonPreload(databaseDirectory / "preload.json", rootData.preload);

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
