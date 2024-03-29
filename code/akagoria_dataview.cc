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

  const char *getShrineType(akgr::ShrineType type) {
    switch (type) {
      case akgr::ShrineType::Ale:
        return "Ale";
      case akgr::ShrineType::Anpa:
        return "Anpa";
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
      case akgr::ShrineType::Wawa:
        return "Wawa";
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
      case akgr::WeaponType::Elemental:
        return "Elemental";
    }

    return "???";
  }



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
      fmt::print("\t\t'{}' {}\n", layer.name, layer.tiles.getSize());
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

  void viewLandscapeData(const akgr::LandscapeData& data) {
    viewNewSection("Landscape");

    fmt::print("Number of shrines: {}\n", data.shrines.size());

    for (auto& shrine : data.shrines) {
      fmt::print("\t{} [{}]\n", shrine.location, getShrineType(shrine.type));
    }
  }

  void viewAreaData(const akgr::Dict<akgr::AreaData>& data) {
    viewNewSection("Areas");
    fmt::print("Number of areas: {}\n", data.size());

    for (auto & area : data) {
      fmt::print("\t{}: '{}' {} {}\n", Id{area.name.id}, area.name.tag, area.position.center, area.position.radius);
    }
  }

  void viewLocationData(const akgr::Dict<akgr::LocationData>& data) {
    viewNewSection("Locations");
    fmt::print("Number of locations: {}\n", data.size());

    for (auto & loc : data) {
      fmt::print("\t{}: '{}' {}\n", Id{loc.name.id}, loc.name.tag, loc.location);
    }
  }

  void viewHeroData(const akgr::HeroData& data) {
    viewNewSection("Hero");

    fmt::print("Number of animations: {}\n", data.animations.size());

    for (auto & animation : data.animations) {
      fmt::print("\t{}: '{}' [{}]\n", Id{animation.name.id}, animation.name.tag, animation.frames.size());

      for (auto & frame : animation.frames) {
        fmt::print("\t\t{} '{}' [{}] {} ms\n", Id{frame.atlas.id}, frame.atlas.tag, frame.index, frame.duration);
      }
    }

  }

  void viewAtlasData(const akgr::Dict<akgr::AtlasData>& data) {
    viewNewSection("Atlases");
    fmt::print("Number of atlases: {}\n", data.size());

    for (auto & atlas : data) {
      fmt::print("\t{}: '{}' '{}' {}\n", Id{atlas.name.id}, atlas.name.tag, atlas.path.string(), atlas.size);
    }
  }


  const char *getDialogType(akgr::DialogType type) {
    switch (type) {
      case akgr::DialogType::Simple:
        return "Simple";
      case akgr::DialogType::Quest:
        return "Quest";
      case akgr::DialogType::Story:
        return "Story";
    }

    return "???";
  }

  void viewDialogData(const akgr::Dict<akgr::DialogData>& data) {
    viewNewSection("Dialogs");
    fmt::print("Number of dialogs: {}\n", data.size());

    for (auto & dialog : data) {
      fmt::print("\t{}: '{}' [{}]\n", Id{dialog.name.id}, dialog.name.tag, getDialogType(dialog.type));

      for (auto& line : dialog.content) {
        fmt::print("\t\t`{}`:\n\t\t\t\"{}\"\n\t\t\t(\"{}\")\n", line.speaker, gf::escapeString(line.words), gf::escapeString(boost::locale::gettext(line.words.c_str())));
      }
    }
  }

  void viewNotificationData(const akgr::Dict<akgr::NotificationData>& data) {
    viewNewSection("Notifications");
    fmt::print("Number of notifications: {}\n", data.size());

    for (auto & notification : data) {
      fmt::print("\t{}: '{}' [{:g} s]: \"{}\" (\"{}\")\n", Id{notification.name.id}, notification.name.tag, notification.duration.asSeconds(), notification.message, gf::escapeString(boost::locale::gettext(notification.message.c_str())));
    }
  }

  void viewCharacterData(const akgr::Dict<akgr::CharacterData>& data) {
    viewNewSection("Characters");
    fmt::print("Number of characters: {}\n", data.size());

    for (auto & character : data) {
      fmt::print("\t{}: '{}' {}\n", Id{character.name.id}, character.name.tag, character.size);
      fmt::print("\t\tattribute {}, level {}, weapon: {} '{}'\n", character.attribute, character.level, Id{character.weapon.id}, character.weapon.tag);
    }
  }

  void viewItemData(const akgr::Dict<akgr::ItemData>&data) {
    viewNewSection("Items");
    fmt::print("Number of items: {}\n", data.size());

    for (auto & item : data) {
      fmt::print("\t{}: '{}', \"{}\", ", Id{item.name.id}, item.name.tag, gf::escapeString(item.description));

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

      fmt::print(", {} '{}' [{}] x{:g}\n", Id{item.sprite.atlas.id}, item.sprite.atlas.tag, item.sprite.index, item.sprite.scale);
    }
  }


  void viewWeaponData(const akgr::Dict<akgr::WeaponData>& data) {
    viewNewSection("Weapons");
    fmt::print("Number of weapons: {}\n", data.size());

    for (auto & weapon : data) {
      fmt::print("\t{}: '{}', {}, \"{}\"\n", Id{weapon.name.id}, weapon.name.tag, getWeaponType(weapon.type), gf::escapeString(weapon.description));
      fmt::print("\t\tattack: {}, attribute: {}, aspect: {} | range: {:g}, angle: {:.0f}°\n", weapon.attack, weapon.attribute, weapon.aspect, weapon.range, weapon.angle);
      fmt::print("\t\twarmup: {} ms | cooldown: {} ms\n", weapon.warmup.asMilliseconds(), weapon.cooldown.asMilliseconds());
    }
  }

  void viewData(const akgr::WorldData& data) {
    viewMapData(data.map);
    viewPhysicsData(data.physics);
    viewLandscapeData(data.landscape);
    viewAreaData(data.areas);
    viewLocationData(data.locations);

    viewHeroData(data.hero);
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
