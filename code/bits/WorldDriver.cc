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
#include "WorldDriver.h"

#include <gf/Clock.h>
#include <gf/Log.h>

#include "WorldConstants.h"

namespace akgr {

  WorldDriver::WorldDriver(const WorldData& data, WorldState& state, WorldScenery& scenery, RootScenery& root, const Commands& commands, Script& script)
  : m_data(data)
  , m_state(state)
  , m_scenery(scenery)
  , m_root(root)
  , m_commands(commands)
  , m_script(script)
  {


  }


  void WorldDriver::processCommands() {
    m_root.helper.status = HelperStatus::None;

    auto& hero = m_state.hero;

    auto squareDistanceToHero = [&hero](gf::Vector2f other) {
      return gf::squareDistance(hero.physics.location.position, other);
    };

    switch (m_state.operation) {
      case WorldOperation::Walk:
        if (m_commands.menu.isActive()) {
          m_scenery.menu.choice = 0;
          m_state.operation = WorldOperation::Menu;
        }

        if (m_commands.right.isActive()) {
          hero.move.angular = gf::AngularMove::Right;
        } else if (m_commands.left.isActive()) {
          hero.move.angular = gf::AngularMove::Left;
        } else {
          hero.move.angular = gf::AngularMove::None;
        }

        if (m_commands.up.isActive()) {
          hero.move.linear = gf::LinearMove::Forward;
        } else if (m_commands.down.isActive()) {
          hero.move.linear = gf::LinearMove::Backward;
        } else {
          hero.move.linear = gf::LinearMove::None;
        }

        if (m_commands.use.isActive()) {
          // check for a character conversation

          for (auto& character : m_state.characters) {
            if (character.dialog == gf::InvalidId) {
              continue;
            }

            if (character.physics.location.floor != hero.physics.location.floor) {
              continue;
            }

            if (squareDistanceToHero(character.physics.location.position) < gf::square(DialogDistance)) {
              m_state.operation = WorldOperation::Talk;

              hero.dialog.ref.id = character.dialog;
              hero.dialog.ref.bind(m_data.dialogs);
              assert(hero.dialog.ref.data != nullptr);

              character.dialog = gf::InvalidId;
            }
          }

          // check for an item

          for (auto& item : m_state.items) {
            if (item.physics.location.floor != hero.physics.location.floor) {
              continue;
            }

            if (squareDistanceToHero(item.physics.location.position) < gf::square(ItemDistance + item.ref.data->shape.getPhysicalSize())) {
              // put in inventory
              hero.inventory.addItem(item.ref);

              // remove from the world
              m_state.physics.world.DestroyBody(item.physics.body);
              item.physics.body = nullptr;
            }
          }

          m_state.items.erase(std::remove_if(m_state.items.begin(), m_state.items.end(), [](const auto& item) { return item.physics.body == nullptr; }), m_state.items.end());

          // check for a shrine

          for (auto& shrine : m_data.shrines) {
            if (shrine.location.floor != hero.physics.location.floor) {
              continue;
            }

            if (squareDistanceToHero(shrine.location.position) < gf::square(ShrineDistance)) {
              switch (shrine.type) {
                case ShrineType::Moli:
                  gf::Log::info("Moli Shrine!\n");
                  break;
                case ShrineType::Pona:
                  hero.attributes.hp.increase();
                  break;
                case ShrineType::Sewi:
                  gf::Log::info("Sewi Shrine!\n");
                  break;
                case ShrineType::Tomo:
                  m_state.operation = WorldOperation::Save;
                  break;
              }

            }
          }


        }
        break;

      case WorldOperation::Talk:
        m_root.helper.status = HelperStatus::Continue;

        hero.move.angular = gf::AngularMove::None;
        hero.move.linear = gf::LinearMove::None;

        if (m_commands.use.isActive()) {
          auto& dialog = hero.dialog;
          assert(dialog.ref.data != nullptr);

          ++dialog.currentLine;

          if (dialog.currentLine >= dialog.ref.data->content.size()) {
            // end of the dialog
            m_state.operation = WorldOperation::Walk;

            std::string name = dialog.ref.data->name;
            dialog.ref.data = nullptr;
            dialog.ref.id = gf::InvalidId;
            dialog.currentLine = 0;
            m_script.onDialog(name);
          }

        }

        break;

      case WorldOperation::Save:
        m_root.operation = RootOperation::SelectSlot;
        m_root.helper.status = HelperStatus::Menu;

        hero.move.angular = gf::AngularMove::None;
        hero.move.linear = gf::LinearMove::None;

        if (m_commands.menuDown.isActive()) {
          m_root.selector.index.computeNextChoice();
        } else if (m_commands.menuUp.isActive()) {
          m_root.selector.index.computePrevChoice();
        }

        if (m_commands.use.isActive()) {
          m_state.operation = WorldOperation::Walk;
          m_root.operation = RootOperation::None;

          if (m_root.selector.index.choice == SlotSelectorScenery::Back) {
            m_root.selector.index.choice = 0;
          } else {
            // TODO: make it async?
            gf::Log::info("Game saving...\n");
            gf::Clock savingClock;

            Slot& slot = m_root.selector.getSlot();
            m_state.saveToFile(slot.path);

            slot.meta.area = m_scenery.area.current->name;
            slot.save();

            auto savingTime = savingClock.getElapsedTime();
            gf::Log::info("Game saved in %d ms\n", savingTime.asMilliseconds());

            m_root.selector.load(); // reload info
          }

        }
        break;

      case WorldOperation::Menu:
        m_root.helper.status = HelperStatus::Menu;

        hero.move.angular = gf::AngularMove::None;
        hero.move.linear = gf::LinearMove::None;

        if (m_commands.menu.isActive()) {
          m_state.operation = WorldOperation::Walk;
        }

        if (m_commands.menuDown.isActive()) {
          m_scenery.menu.computeNextChoice();
        } else if (m_commands.menuUp.isActive()) {
          m_scenery.menu.computePrevChoice();
        }

        if (m_commands.use.isActive()) {
          switch (m_scenery.menu.choice) {
            case GameMenuScenery::Inventory:
              m_state.operation = WorldOperation::Inventory;
              m_scenery.inventory.size = m_state.hero.inventory.items.size();
              break;

            case GameMenuScenery::Quests:
              // TODO
              gf::Log::debug("Quests\n");
              break;

            case GameMenuScenery::Skills:
              // TODO
              gf::Log::debug("Skills\n");
              break;

            case GameMenuScenery::Options:
              m_state.operation = WorldOperation::Options;
              break;

            case GameMenuScenery::Quit:
              m_state.operation = WorldOperation::Walk;
              break;
          }

        }
        break;

      case WorldOperation::Options:
        m_root.operation = RootOperation::ChangeOptions;
        m_root.helper.status = HelperStatus::Menu;

        if (m_commands.menu.isActive()) {
          m_state.operation = WorldOperation::Walk;
        }

        if (m_commands.menuDown.isActive()) {
          m_root.options.index.computeNextChoice();
        } else if (m_commands.menuUp.isActive()) {
          m_root.options.index.computePrevChoice();
        }

        if (m_commands.use.isActive()) {
          if (m_root.options.getChoice() == OptionsScenery::Choice::Back) {
            m_state.operation = WorldOperation::Menu;
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

      case WorldOperation::Inventory:
        m_root.helper.status = HelperStatus::Inventory;

        if (m_commands.menuDown.isActive()) {
          m_scenery.inventory.computeNextItem();
        } else if (m_commands.menuUp.isActive()) {
          m_scenery.inventory.computePrevItem();
        }

        if (m_commands.menuPageDown.isActive()) {
          for (std::size_t i = 0; i < InventoryScenery::Length / 2; ++i) {
            m_scenery.inventory.computeNextItem();
          }
        } else if (m_commands.menuPageUp.isActive()) {
          for (std::size_t i = 0; i < InventoryScenery::Length / 2; ++i) {
            m_scenery.inventory.computePrevItem();
          }
        }

        if (m_commands.use.isActive()) {
//           m_state.operation = WorldOperation::Menu;
        }

        if (m_commands.menuQuit.isActive()) {
          m_state.operation = WorldOperation::Menu;
        }
        break;
    }
  }

}
