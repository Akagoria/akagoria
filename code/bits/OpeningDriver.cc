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
#include "OpeningDriver.h"

#include <gf/Log.h>

namespace akgr {

  OpeningDriver::OpeningDriver(OpeningScenery& scenery, const Commands& commands)
  : m_scenery(scenery)
  , m_commands(commands)
  {

  }

  void OpeningDriver::processCommands() {
    switch (m_scenery.operation) {
      case OpeningOperation::Menu:
        if (m_commands.menuDown.isActive()) {
          m_scenery.menu.choice = (m_scenery.menu.choice + 1) % StartMenuScenery::ItemCount;
        } else if (m_commands.menuUp.isActive()) {
          m_scenery.menu.choice = (m_scenery.menu.choice - 1 + StartMenuScenery::ItemCount) % StartMenuScenery::ItemCount;
        }

        if (m_commands.use.isActive()) {
          switch (m_scenery.menu.choice) {
            case StartMenuScenery::StartAdventure:
              m_scenery.operation = OpeningOperation::Start;
              break;

            case StartMenuScenery::LoadAdventure:
              m_scenery.operation = OpeningOperation::Select;
              break;

            case StartMenuScenery::Quit:
              m_scenery.operation = OpeningOperation::Quit;
              break;
          }
        }
        break;

      case OpeningOperation::Select:
        if (m_commands.menuDown.isActive()) {
          m_scenery.selector.choice = (m_scenery.selector.choice + 1) % SlotSelectorScenery::ItemCount;
        } else if (m_commands.menuUp.isActive()) {
          m_scenery.selector.choice = (m_scenery.selector.choice - 1 + SlotSelectorScenery::ItemCount) % SlotSelectorScenery::ItemCount;
        }

        if (m_commands.use.isActive()) {
          if (m_scenery.selector.choice == SlotSelectorScenery::Back) {
            m_scenery.operation = OpeningOperation::Menu;
            m_scenery.selector.choice = 0;
          } else {
            if (m_scenery.selector.getSlot().active) {
              m_scenery.operation = OpeningOperation::Load;
            }
          }
        }
        break;

      default:
        break;
    }
  }

}

