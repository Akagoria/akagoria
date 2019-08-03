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
        fmt::print("\t\t\t'{}' {}, {:.0f}°, tileset #{}, {}\n", sprite.name, sprite.position, sprite.rotation, sprite.tilesetId, sprite.subTexture);
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
      fmt::print("\t#{}: '{}', {}, {}, {}\n", id, tileset.path.string(), tileset.tileSize, tileset.spacing, tileset.margin);
      id++;
    }

  }

  const char *getShapeType(akgr::ShapeType type) {
    switch (type) {
      case akgr::ShapeType::None:
        return "None";
      case akgr::ShapeType::Circle:
        return "Circle";
      case akgr::ShapeType::Rectangle:
        return "Rectangle";
    }

    return "???";
  }

  const char *getWeaponType(akgr::WeaponType type) {
    switch (type) {
      case akgr::WeaponType::Melee:
        return "Melee";
      case akgr::WeaponType::Ranged:
        return "Ranged";
      case akgr::WeaponType::Explosive:
        return "Explosive";
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

    fmt::print("Number of things: {}\n", data.things.size());

    for (auto& thing : data.things) {
      fmt::print("\t'{}' {} {:.0f}° + {} of {} points\n", thing.name, thing.location, thing.angle, thing.line.isChain() ? "chain" : "loop", thing.line.getPointCount());
    }
  }

  void viewAreaData(const std::map<gf::Id, akgr::AreaData>& data) {
    viewNewSection("Areas");
    fmt::print("Number of areas: {}\n", data.size());

    for (auto& kv : data) {
      auto& area = kv.second;
      fmt::print("\t{}: '{}' {} {}\n", Id{kv.first}, area.name, area.position.center, area.position.radius);
    }
  }

  void viewLocationData(const std::map<gf::Id, akgr::LocationData>& data) {
    viewNewSection("Locations");
    fmt::print("Number of locations: {}\n", data.size());

    for (auto& kv : data) {
      auto& loc = kv.second;
      fmt::print("\t{}: '{}' {}\n", Id{kv.first}, loc.name, loc.location);
    }
  }

  const char *getShrineType(akgr::ShrineType type) {
    switch (type) {
      case akgr::ShrineType::Ale:
        return "Ale";
      case akgr::ShrineType::Ike:
        return "Ike";
      case akgr::ShrineType::Moli:
        return "Moli";
      case akgr::ShrineType::Pona:
        return "Pona";
      case akgr::ShrineType::Sewi:
        return "Sewi";
      case akgr::ShrineType::Sijelo:
        return "Sijelo";
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

  void viewAtlasData(const std::map<gf::Id, akgr::AtlasData>& data) {
    viewNewSection("Atlases");
    fmt::print("Number of atlases: {}\n", data.size());

    for (auto& kv : data) {
      auto& atlas = kv.second;
      fmt::print("\t{}: '{}' {}\n", Id{kv.first}, atlas.path.string(), atlas.size);
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

    for (auto& kv : data) {
      auto& dialog = kv.second;
      fmt::print("\t{}: '{}' [{}]\n", Id{kv.first}, dialog.name, getDialogType(dialog.type));

      for (auto& line : dialog.content) {
        fmt::print("\t\t`{}`:\n\t\t\t\"{}\"\n\t\t\t(\"{}\")\n", line.speaker, gf::escapeString(line.words), gf::escapeString(boost::locale::gettext(line.words.c_str())));
      }
    }
  }

  void viewNotificationData(const std::map<gf::Id, akgr::NotificationData>& data) {
    viewNewSection("Notifications");
    fmt::print("Number of notifications: {}\n", data.size());

    for (auto& kv : data) {
      auto& notification = kv.second;
      fmt::print("\t{}: '{}' [{:g} s]: '{}'\n", Id{kv.first}, notification.name, notification.duration.asSeconds(), notification.message);
    }
  }

  void viewCharacterData(const std::map<gf::Id, akgr::CharacterData>& data) {
    viewNewSection("Characters");
    fmt::print("Number of characters: {}\n", data.size());

    for (auto& kv : data) {
      auto& character = kv.second;
      fmt::print("\t{}: '{}' {}\n", Id{kv.first}, character.name, character.size);
    }
  }

  void viewItemData(const std::map<gf::Id, akgr::ItemData>&data) {
    viewNewSection("Items");
    fmt::print("Number of items: {}\n", data.size());

    for (auto& kv : data) {
      auto& item = kv.second;
      fmt::print("\t{}: '{}', \"{}\", ", Id{kv.first}, item.name, gf::escapeString(item.description));

      fmt::print("{} ", getShapeType(item.shape.type));

      switch (item.shape.type) {
        case akgr::ShapeType::None:
          break;
        case akgr::ShapeType::Circle:
          fmt::print("radius: {}", item.shape.circle.radius);
          break;
        case akgr::ShapeType::Rectangle:
          fmt::print("width: {}, height: {}", item.shape.rectangle.width, item.shape.rectangle.height);
          break;
      }

      fmt::print(", {} [{}] x{:g}\n", Id{item.sprite.atlas}, item.sprite.index, item.sprite.scale);
    }
  }


  void viewWeaponData(const std::map<gf::Id, akgr::WeaponData>& data) {
    viewNewSection("Weapons");
    fmt::print("Number of weapons: {}\n", data.size());

    for (auto& kv : data) {
      auto& weapon = kv.second;
      fmt::print("\t{}: '{}' {}, \"{}\"\n", Id{kv.first}, weapon.name, getWeaponType(weapon.type), gf::escapeString(weapon.description));
      fmt::print("\t\tATK: {:g}, REQ: {:g}, VP: {:g} | range: {:g}, angle: {:.0f}° | cooldown: {} ms\n",
          weapon.attack, weapon.required, weapon.vitality, weapon.range, weapon.angle, weapon.cooldown.asMilliseconds());
    }
  }

  void viewData(const akgr::WorldData& data) {
    viewMapData(data.map);
    viewPhysicsData(data.physics);
    viewAreaData(data.areas);
    viewLocationData(data.locations);
    viewShrineData(data.shrines);

    viewAtlasData(data.atlases);
    viewDialogData(data.dialogs);
    viewNotificationData(data.notifications);
    viewCharacterData(data.characters);
    viewItemData(data.items);
    viewWeaponData(data.weapons);
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
