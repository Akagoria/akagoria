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
#include "bits/UIData.h"

namespace {
  void viewNewSection(const char *section) {
    fmt::print("#\n# {}\n#\n", section);
  }

  void viewUIMessageData(const std::map<gf::Id, akgr::UIMessageData>& data) {
    viewNewSection("UI Messages");
    fmt::print("Number of UI messages: {}\n", data.size());

    for (auto& item : data) {
      auto& ui = item.second;
      fmt::print("\t{}: '{}': '{}'\n", Id{item.first}, ui.name, ui.message);
    }
  }

  void viewUIData(const akgr::UIData& data) {
    viewUIMessageData(data.messages);
  }

}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fmt::print("Usage: alagoria_uimessagesview <file>\n");
    return EXIT_FAILURE;
  }

  fmt::print("Loading akagoria ui messages from '{}'...\n", argv[1]);
  gf::Path inputFile(argv[1]);

  akgr::UIData data;

  gf::Clock clock;

  if (!data.loadFromFile(inputFile)) {
    fmt::print(stderr, "Could not load '{}'\n", argv[1]);
    return EXIT_FAILURE;
  }

  auto duration = clock.getElapsedTime();
  fmt::print("Data successfully loaded in {:g} s\n", duration.asSeconds());

  viewUIData(data);

  return EXIT_SUCCESS;
}
