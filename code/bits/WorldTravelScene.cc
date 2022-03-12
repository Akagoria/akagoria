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
#include "WorldTravelScene.h"

#include <gf/Log.h>
#include <gf/Unused.h>

#include "Akagoria.h"
#include "WorldAct.h"
#include "WorldConstants.h"

namespace akgr {

  namespace {

    constexpr gf::Time AreaUpdatePeriod = gf::seconds(1);

  }

  WorldTravelScene::WorldTravelScene(Akagoria& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_area(game.resources, game.world.state, game.world.scenery)
  , m_aspect(game.world.state, game.resources)
  , m_map(game.world.state)
  , m_notifications(game.world.state, game.theme)
  {
    addHudEntity(m_area);
    addHudEntity(m_aspect);
    addHudEntity(m_map);
    addHudEntity(m_notifications);

    addAction(m_game.commands.gameUp);
    addAction(m_game.commands.gameDown);
    addAction(m_game.commands.gameLeft);
    addAction(m_game.commands.gameRight);
    addAction(m_game.commands.gameMove);
    addAction(m_game.commands.gameUse);
    addAction(m_game.commands.gameFight);
    addAction(m_game.commands.gameMenu);

    setClearColor(gf::Color::Black);
  }

  void WorldTravelScene::doHandleActions(gf::Window& window) {
    gf::unused(window);

    auto& hero = m_game.world.state.hero;

    auto squareDistanceToHero = [&hero](gf::Vector2f other) {
      return gf::squareDistance(hero.physics.location.position, other);
    };

    if (m_game.commands.gameMenu.isActive()) {
      m_game.world.scenery.menu.index.choice = 0;
      m_game.replaceScene(m_game.worldAct->menu);
    }

    if (m_game.commands.gameRight.isActive()) {
      hero.move.method = HeroMoveMethod::Relative;
      hero.move.angular = gf::AngularMove::Right;
    } else if (m_game.commands.gameLeft.isActive()) {
      hero.move.method = HeroMoveMethod::Relative;
      hero.move.angular = gf::AngularMove::Left;
    } else {
      hero.move.angular = gf::AngularMove::None;
    }

    if (m_game.commands.gameUp.isActive()) {
      hero.move.method = HeroMoveMethod::Relative;
      hero.move.linear = gf::LinearMove::Forward;
    } else if (m_game.commands.gameDown.isActive()) {
      hero.move.method = HeroMoveMethod::Relative;
      hero.move.linear = gf::LinearMove::Backward;
    } else {
      hero.move.linear = gf::LinearMove::None;
    }

    if (m_game.commands.gameMove.isActive()) {
      hero.move.method = HeroMoveMethod::Absolute;
      hero.move.linear = gf::LinearMove::Forward;
      hero.move.angular = gf::AngularMove::None;
      hero.physics.angle = m_game.commands.gamepad.getAngle() + gf::Pi2;
    } else {
      if (hero.move.method == HeroMoveMethod::Absolute) {
        hero.move.linear = gf::LinearMove::None;
      }
    }

    if (m_game.commands.gameUse.isActive()) {
      // check for a character conversation

      for (auto& character : m_game.world.state.characters) {
        if (character.dialog.id == gf::InvalidId) {
          continue;
        }

        if (character.physics.location.floor != hero.physics.location.floor) {
          continue;
        }

        if (squareDistanceToHero(character.physics.location.position) < gf::square(DialogDistance)) {
          hero.dialog.ref = character.dialog;
          assert(hero.dialog.ref.data != nullptr);

          character.dialog.id = gf::InvalidId;
          character.dialog.data = nullptr;

          m_game.replaceScene(m_game.worldAct->dialog);
        }
      }

      // check for an item

      auto& items = m_game.world.state.items;

      for (auto& item : items) {
        if (item.physics.location.floor != hero.physics.location.floor) {
          continue;
        }

        if (squareDistanceToHero(item.physics.location.position) < gf::square(ItemDistance + item.ref().shape.getPhysicalSize())) {
          // put in inventory
          hero.inventory.addItem(item.ref);

          // remove from the world
          m_game.world.state.physics.model.world.DestroyBody(item.physics.body);
          item.physics.body = nullptr;
        }
      }

      items.erase(std::remove_if(items.begin(), items.end(), [](const auto& item) { return item.physics.body == nullptr; }), items.end());

      // check for a shrine

      for (auto& shrine : m_game.world.data.landscape.shrines) {
        if (shrine.location.floor != hero.physics.location.floor) {
          continue;
        }

        if (squareDistanceToHero(shrine.location.position) < gf::square(ShrineDistance)) {
          switch (shrine.type) {
            case ShrineType::Ale:
              m_game.replaceScene(m_game.worldAct->selector);
              break;
            case ShrineType::Ike:
              gf::Log::info("Ike Shrine!\n");
              break;
            case ShrineType::Moli:
              gf::Log::info("Moli Shrine!\n");
              break;
            default:
              break;
          }

        }
      }
    }

    if (m_game.commands.gameFight.isActive()) {
      if (hero.weapon.phase == WeaponPhase::Ready) {
        hero.weapon.phase = WeaponPhase::WarmUp;
      }
    }
  } // end of doHandleActions

  void WorldTravelScene::doUpdate(gf::Time time) {
    auto &hero = m_game.world.state.hero;

    // notifications

    if (!m_game.world.state.notifications.empty()) {
      auto& current = m_game.world.state.notifications.front();
      current.elapsed += time;

      if (current.elapsed > current.ref().duration) {
        m_game.world.state.notifications.erase(m_game.world.state.notifications.begin());
      }
    }

    // area

    m_game.world.scenery.area.period += time;

    if (!m_game.world.data.areas.empty() && (m_game.world.scenery.area.current == nullptr || m_game.world.scenery.area.period > AreaUpdatePeriod)) {
      m_game.world.scenery.area.period -= AreaUpdatePeriod;

      auto distanceToHero = [&hero](const auto& area) {
        return gf::naturalDistance(hero.physics.location.position, area.position.center);
      };

      auto it = std::min_element(m_game.world.data.areas.begin(), m_game.world.data.areas.end(), [&](const auto& lhs, const auto& rhs) {
        return distanceToHero(lhs) < distanceToHero(rhs);
      });

      m_game.world.scenery.area.current = std::addressof(*it);
    }
  } // end of doUpdate

}
