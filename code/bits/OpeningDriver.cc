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

  OpeningDriver::OpeningDriver(OpeningScenery& scenery, RootScenery& root, const Commands& commands)
  : m_scenery(scenery)
  , m_root(root)
  , m_commands(commands)
  {

  }

  void OpeningDriver::processCommands() {
    m_root.helper.status = HelperStatus::Menu;

    switch (m_scenery.operation) {
      case OpeningOperation::Menu:
        if (m_commands.menuDown.isActive()) {
          m_scenery.menu.computeNextChoice();
        } else if (m_commands.menuUp.isActive()) {
          m_scenery.menu.computePrevChoice();
        }

        if (m_commands.use.isActive()) {
          switch (m_scenery.menu.choice) {
            case StartMenuScenery::StartAdventure:
              m_scenery.operation = OpeningOperation::Start;
              break;

            case StartMenuScenery::LoadAdventure:
              m_scenery.operation = OpeningOperation::SelectSlot;
              m_root.operation = RootOperation::SelectSlot;
              break;

            case StartMenuScenery::Options:
              m_scenery.operation = OpeningOperation::ChangeOptions;
              m_root.operation = RootOperation::ChangeOptions;
              break;

            case StartMenuScenery::Quit:
              m_scenery.operation = OpeningOperation::Quit;
              break;
          }
        }
        break;

      case OpeningOperation::SelectSlot:
        if (m_commands.menuDown.isActive()) {
          m_root.selector.computeNextChoice();
        } else if (m_commands.menuUp.isActive()) {
          m_root.selector.computePrevChoice();
        }

        if (m_commands.use.isActive()) {
          if (m_root.selector.choice == SlotSelectorScenery::Back) {
            m_scenery.operation = OpeningOperation::Menu;
            m_root.selector.choice = 0;
            m_root.operation = RootOperation::None;
          } else {
            if (m_root.selector.getSlot().active) {
              m_scenery.operation = OpeningOperation::Load;
              m_root.operation = RootOperation::None;
            }
          }
        }
        break;

      case OpeningOperation::ChangeOptions:
        if (m_commands.menuDown.isActive()) {
          m_root.options.computeNextChoice();
        } else if (m_commands.menuUp.isActive()) {
          m_root.options.computePrevChoice();
        }

        if (m_commands.use.isActive()) {
          if (m_root.options.choice == OptionsScenery::Back) {
            m_scenery.operation = OpeningOperation::Menu;
            m_root.operation = RootOperation::None;
            m_root.options.save();
          }
        }

        if (m_commands.menuRight.isActive()) {
          m_root.options.computeNextOption();
        }

        if (m_commands.menuLeft.isActive()) {
          m_root.options.computePrevOption();
        }
        break;

      default:
        break;
    }
  }

}

