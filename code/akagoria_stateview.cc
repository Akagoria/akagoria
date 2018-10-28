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

#include <gf/Clock.h>
#include <gf/Path.h>

#include <fmt/core.h>

#include "bits/Fmt.h"
#include "bits/WorldState.h"

namespace {

  void viewNewSection(const char *section) {
    fmt::print("#\n# {}\n#\n", section);
  }

//   void viewPhysicsState(const akgr::PhysicsState& state) {
//     viewNewSection("Physics");
//
//   }

  void viewNotificationsState(const std::vector<akgr::NotificationState>& state) {
    viewNewSection("Notifications");
    fmt::print("Number of notifications: {}\n", state.size());

    for (auto& item : state) {
      fmt::print("\t{}: {:g} s\n", Id{item.ref.id}, item.elapsed.asSeconds());
    }
  }

  void viewCharactersState(const std::vector<akgr::CharacterState>& state) {
    viewNewSection("Characters");
    fmt::print("Number of characters: {}\n", state.size());

    for (auto& item : state) {
      fmt::print("\t{}: ({}) {} {:.0f}°\n", Id{item.ref.id}, Id{item.dialog},  item.physics.location, gf::radiansToDegrees(item.physics.angle));
    }
  }

  const char *getHeroOperation(akgr::HeroOperation op) {
    switch (op) {
      case akgr::HeroOperation::Walk:
        return "Walk";
      case akgr::HeroOperation::Talk:
        return "Talk";
      case akgr::HeroOperation::Save:
        return "Save";
    }

    assert(false);
    return "???";
  }

  const char *getLinearMove(gf::LinearMove move) {
    switch (move) {
      case gf::LinearMove::None:
        return "None";
      case gf::LinearMove::Forward:
        return "Forward";
      case gf::LinearMove::Backward:
        return "Backward";
    }

    assert(false);
    return "???";
  }

  const char *getAngularMove(gf::AngularMove move) {
    switch (move) {
      case gf::AngularMove::None:
        return "None";
      case gf::AngularMove::Left:
        return "Left";
      case gf::AngularMove::Right:
        return "Right";
    }

    assert(false);
    return "???";
  }

  void viewHeroState(const akgr::HeroState& state) {
    viewNewSection("Hero");

    fmt::print("\top: {}, move: {} {}\n", getHeroOperation(state.operation), getLinearMove(state.move.linear), getAngularMove(state.move.angular));
    fmt::print("\trequirements:");

    for (auto id : state.requirements) {
      fmt::print(" {}", Id{ id });
    }

    fmt::print("\n");

    fmt::print("\tattributes: {}/{} HP ({:g} s) {}/{} MP ({:g} s)\n",
        state.attributes.hp.current, state.attributes.hp.max, state.attributes.hp.period.asSeconds(),
        state.attributes.mp.current, state.attributes.mp.max, state.attributes.mp.period.asSeconds()
    );

    fmt::print("\tdialog: {} line {}\n", Id{ state.dialog.ref.id }, state.dialog.currentLine);

    fmt::print("\tphysics: {} {:.0f}°\n", state.physics.location, gf::radiansToDegrees(state.physics.angle));
  }

  void viewState(const akgr::WorldState& state) {
//     viewPhysicsState(state.physics);
    viewNotificationsState(state.notifications);
    viewCharactersState(state.characters);
    viewHeroState(state.hero);
  }

}


int main(int argc, char *argv[]) {
  if (argc != 2) {
    fmt::print("Usage: alagoria_dataview <file>\n");
    return EXIT_FAILURE;
  }

  fmt::print("Loading akagoria state from '{}'...\n", argv[1]);
  gf::Path inputFile(argv[1]);

  akgr::WorldState state;

  gf::Clock clock;

  if (!state.loadFromFile(inputFile)) {
    fmt::print(stderr, "Could not load '{}'\n", argv[1]);
    return EXIT_FAILURE;
  }

  auto duration = clock.getElapsedTime();
  fmt::print("State successfully loaded in {:g} s\n", duration.asSeconds());

  viewState(state);

  return EXIT_SUCCESS;
}
