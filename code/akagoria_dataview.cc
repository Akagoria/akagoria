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
#include <cinttypes>
#include <cstdio>
#include <cstdlib>

#include <boost/locale.hpp>

#include <gf/Clock.h>
#include <gf/Path.h>
#include <gf/StringUtils.h>

#include <fmt/core.h>

#include "bits/Fmt.h"
#include "bits/WorldData.h"

#include "config.h"

namespace {

  void viewNewSection(const char *section) {
    fmt::print("#\n# {}\n#\n", section);
  }

  void viewMapSpriteLayer(const akgr::SpriteLayer& layer) {
    if (!layer.sprites.empty()) {
      fmt::print("\t\t'{}' ({})\n", layer.name, layer.sprites.size());

      for (auto& sprite : layer.sprites) {
        fmt::print("\t\t\t'{}' {}, {}°, tileset #{}, {}\n", sprite.name, sprite.position, sprite.rotation, sprite.tilesetId, sprite.subTexture);
      }
    }
  }

  void viewMapTextureLayer(const akgr::TextureLayer& layer) {
    if (!layer.tiles.isEmpty()) {
      fmt::print("\t\t'{}' {}, tileset #{}\n", layer.name, layer.tiles.getSize(), layer.tilesetId);
    }
  }

  void viewMapData(const akgr::MapData& data) {
    viewNewSection("Map");
    fmt::print("Properties:\n");
    fmt::print("\tMap size: {}\n", data.mapSize);
    fmt::print("\tTile size: {}\n", data.tileSize);
    fmt::print("Number of floors: {}\n", data.floors.size());

    for (auto floor = data.floorMin; floor <= data.floorMax; ++floor) {
      const akgr::Floor& current = data[floor];

      fmt::print("\tFloor {}\n", floor);

      viewMapTextureLayer(current.groundTiles);
      viewMapTextureLayer(current.lowTiles);
      viewMapTextureLayer(current.highTiles);
      viewMapSpriteLayer(current.lowSprites);
      viewMapSpriteLayer(current.highSprites);
    }

    fmt::print("Number of tilesets: {}\n", data.tilesets.size());
    uint32_t id = 0;

    for (auto& tileset : data.tilesets) {
      fmt::print("\t#{}: '{}', {}, {}\n", id, tileset.path.string(), tileset.spacing, tileset.margin);
      id++;
    }

  }

  const char *getPhysicsShapeType(akgr::PhysicsShapeType type) {
    switch (type) {
      case akgr::PhysicsShapeType::Circle:
        return "Circle";
      case akgr::PhysicsShapeType::Rectangle:
        return "Rectangle";
    }

    return "???";
  }

  void viewPhysicsData(const akgr::PhysicsData& data) {
    viewNewSection("Physics");
    fmt::print("Number of zones: {}\n", data.zones.size());

    for (auto& zone : data.zones) {
      fmt::print("\t'{}' {} + {} points ", zone.name, zone.location, zone.polygon.getPointCount());

      fmt::print("[");
      for (auto requirement : zone.requirements) {
        fmt::print(" {}", Id{requirement});
      }
      fmt::print(" ] --> '{}'\n", zone.message);
    }

    fmt::print("Number of collisions: {}\n", data.collisions.size());

    for (auto& collision : data.collisions) {
      fmt::print("\t'{}' {} + {} of {} points\n", collision.name, collision.location, collision.line.isChain() ? "chain" : "loop", collision.line.getPointCount());
    }

    fmt::print("Number of shapes: {}\n", data.shapes.size());

    for (auto& shape : data.shapes) {
      fmt::print("\t'{}' {} {} ", shape.name, shape.location, getPhysicsShapeType(shape.type));

      switch (shape.type) {
        case akgr::PhysicsShapeType::Circle:
          fmt::print("radius: {}\n", shape.circle.radius);
          break;
        case akgr::PhysicsShapeType::Rectangle:
          fmt::print("width: {}, height: {}\n", shape.rectangle.width, shape.rectangle.height);
          break;
      }
    }
  }

  void viewAreaData(const std::map<gf::Id, akgr::AreaData>& data) {
    viewNewSection("Areas");
    fmt::print("Number of areas: {}\n", data.size());

    for (auto& item : data) {
      auto& area = item.second;
      fmt::print("\t{}: '{}' {} {}\n", Id{item.first}, area.name, area.position.center, area.position.radius);
    }
  }

  void viewLocationData(const std::map<gf::Id, akgr::LocationData>& data) {
    viewNewSection("Locations");
    fmt::print("Number of locations: {}\n", data.size());

    for (auto& item : data) {
      auto& loc = item.second;
      fmt::print("\t{}: '{}' {}\n", Id{item.first}, loc.name, loc.location);
    }
  }

  const char *getShrineType(akgr::ShrineType type) {
    switch (type) {
      case akgr::ShrineType::Moli:
        return "Moli";
      case akgr::ShrineType::Pona:
        return "Pona";
      case akgr::ShrineType::Sewi:
        return "Sewi";
      case akgr::ShrineType::Tomo:
        return "Tomo";
    }

    return "???";
  }

  void viewShrineData(const std::vector<akgr::ShrineData>& data) {
    viewNewSection("Shrines");
    fmt::print("Number of shrines: {}\n", data.size());

    for (auto& shrine : data) {
      fmt::print("\t{} [{}]\n", shrine.location, getShrineType(shrine.type));
    }
  }

  const char *getDialogType(akgr::DialogData::Type type) {
    switch (type) {
      case akgr::DialogData::Simple:
        return "Simple";
      case akgr::DialogData::Quest:
        return "Quest";
      case akgr::DialogData::Story:
        return "Story";
    }

    return "???";
  }

  void viewDialogData(const std::map<gf::Id, akgr::DialogData>& data) {
    viewNewSection("Dialogs");
    fmt::print("Number of dialogs: {}\n", data.size());

    for (auto& item : data) {
      auto& dialog = item.second;
      fmt::print("\t{}: '{}' [{}]\n", Id{item.first}, dialog.name, getDialogType(dialog.type));

      for (auto& line : dialog.content) {
        fmt::print("\t\t`{}`:\n\t\t\t\"{}\"\n\t\t\t(\"{}\")\n", line.speaker, gf::escapeString(line.words), gf::escapeString(boost::locale::gettext(line.words.c_str())));
      }
    }
  }

  void viewNotificationData(const std::map<gf::Id, akgr::NotificationData>& data) {
    viewNewSection("Notifications");
    fmt::print("Number of notifications: {}\n", data.size());

    for (auto& item : data) {
      auto& notification = item.second;
      fmt::print("\t{}: '{}' [{:g} s]: '{}'\n", Id{item.first}, notification.name, notification.duration.asSeconds(), notification.message);
    }
  }


  void viewCharacterData(const std::map<gf::Id, akgr::CharacterData>& data) {
    viewNewSection("Characters");
    fmt::print("Number of characters: {}\n", data.size());

    for (auto& item : data) {
      auto& character = item.second;
      fmt::print("\t{}: '{}' {}\n", Id{item.first}, character.name, character.size);
    }
  }

  void viewData(const akgr::WorldData& data) {
    viewMapData(data.map);
    viewPhysicsData(data.physics);
    viewAreaData(data.areas);
    viewLocationData(data.locations);
    viewShrineData(data.shrines);
    viewDialogData(data.dialogs);
    viewNotificationData(data.notifications);
    viewCharacterData(data.characters);
  }

}


int main(int argc, char *argv[]) {
  if (argc != 2) {
    fmt::print("Usage: alagoria_dataview <file>\n");
    return EXIT_FAILURE;
  }

  boost::locale::generator localeGenerator;
  localeGenerator.add_messages_path(AKAGORIA_LOCALEDIR);
  localeGenerator.add_messages_domain("akagoria");
  std::locale::global(localeGenerator(""));

  fmt::print("Loading akagoria data from '{}'...\n", argv[1]);
  gf::Path inputFile(argv[1]);

  akgr::WorldData data;

  gf::Clock clock;

  if (!data.loadFromFile(inputFile)) {
    fmt::print(stderr, "Could not load '{}'\n", argv[1]);
    return EXIT_FAILURE;
  }

  auto duration = clock.getElapsedTime();
  fmt::print("Data successfully loaded in {:g} s\n", duration.asSeconds());

  viewData(data);

  return EXIT_SUCCESS;
}
