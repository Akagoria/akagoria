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
#include "WorldBaseScene.h"

#include <gf/Log.h>

#include "Akagoria.h"
#include "WorldAct.h"
#include "WorldConstants.h"

namespace akgr {

  namespace {
    constexpr gf::Vector2f ViewSize(800.0f, 800.0f);

    constexpr gf::Time HPUpdatePeriod = gf::seconds(11);
    constexpr gf::Time MPUpdatePeriod = gf::seconds(29);
    constexpr gf::Time VPUpdatePeriod = gf::seconds(29);

    bool isAspectShrine(ShrineType shrine) {
      switch (shrine) {
        case ShrineType::Pona:
        case ShrineType::Wawa:
        case ShrineType::Sijelo:
          return true;
        default:
          break;
      }

      return false;
    }

    Aspect getAspectFromShrine(ShrineType shrine) {
      switch (shrine) {
        case ShrineType::Pona:
          return Aspect::Health;
        case ShrineType::Wawa:
          return Aspect::Magic;
        case ShrineType::Sijelo:
          return Aspect::Vitality;
        default:
          break;
      }

      assert(false);
      return Aspect::Health;
    }

    Aspect getAspectFromWeaponType(WeaponType type) {
      switch (type) {
        case WeaponType::Melee:
        case WeaponType::Ranged:
        case WeaponType::Explosive:
          return Aspect::Vitality;
        case WeaponType::Elemental:
          return Aspect::Magic;
      }

      assert(false);
      return Aspect::Health;
    }

    Attribute getAttributeFromWeaponType(WeaponType type) {
      switch (type) {
        case WeaponType::Melee:
          return Attribute::Strength;
        case WeaponType::Ranged:
          return Attribute::Dexterity;
        case WeaponType::Explosive:
          return Attribute::Knowledge;
        case WeaponType::Elemental:
          return Attribute::Intelligence;
      }

      assert(false);
      return Attribute::Strength;
    }


    gf::Vector2f computeDamagePosition(gf::Vector2f attacker, gf::Vector2f defender) {
      return defender + gf::normalize(defender - attacker) * 60.0f;
    }

  }

  WorldBaseScene::WorldBaseScene(Akagoria& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_universe(game.world.data, game.world.state, game.resources, game.messages, game.random)
  , m_helper(game.root.data, game.root.scenery, game.resources)
  , m_debug(game.world.state.physics.model)
  , m_listener(game.world.script)
  {
    addWorldEntity(m_universe);
    addHudEntity(m_helper);

    addWorldEntity(m_debug);

    addAction(m_game.commands.windowClose);
    addAction(m_game.commands.windowFullscreen);

    addAction(m_game.commands.debugPhysics);
    addAction(m_game.commands.debugSave);

    m_game.world.state.physics.model.world.SetContactListener(&m_listener);

    setWorldViewSize(ViewSize);
  }


  /*
   * ##########################################################################
   */


  void WorldBaseScene::doHandleActions(gf::Window& window) {
    m_game.root.scenery.helper.status = HelperStatus::None;

    if (m_game.commands.windowFullscreen.isActive()) {
      window.toggleFullscreen();
    }

    if (m_game.commands.windowClose.isActive()) {
      window.close();
    }

    if (m_game.commands.debugPhysics.isActive()) {
      m_debug.toggleDebug();
    }

    if (m_game.commands.debugSave.isActive()) {
      m_game.slots.saveInLastSlot(m_game.world.data, m_game.world.state);
      gf::Log::debug("Quick save\n");
    }

  } // end of doHandleActions


  /*
   * ##########################################################################
   */


  void WorldBaseScene::doUpdate(gf::Time time) {
    float dt = time.asSeconds();

    /*
     * state
     */

    auto &hero = m_game.world.state.hero;
    const uint16_t floor = hero.physics.location.floor;

    auto squareDistanceToHero = [&hero](gf::Vector2f other) {
      return gf::squareDistance(hero.physics.location.position, other);
    };

    // hero (pre-update)
    static constexpr float Hop = 150.0f;
    static constexpr float Turn = 3.0f;

    if (hero.move.method == HeroMoveMethod::Relative) {
      float angle = hero.physics.angle;
      float velocity = 0.0f;

      switch (hero.move.angular) {
        case gf::AngularMove::None:
          break;

        case gf::AngularMove::Left:
          angle -= dt * Turn;
          break;

        case gf::AngularMove::Right:
          angle += dt * Turn;
          break;
      }

      switch (hero.move.linear) {
        case gf::LinearMove::None:
          break;

        case gf::LinearMove::Forward:
          velocity = -Hop;
          break;

        case gf::LinearMove::Backward:
          velocity = Hop * 0.5f;
          break;
      }

      angle = std::fmod(angle, 2 * gf::Pi);
      hero.physics.setAngleAndVelocity(angle, velocity);
    } else {
      float angle = hero.physics.angle;
      float velocity = 0.0f;

      if (hero.move.linear == gf::LinearMove::Forward) {
        velocity = -Hop;
      }

      hero.physics.setAngleAndVelocity(angle, velocity);
    }

    // physics

    m_game.world.state.physics.update(time);
    m_game.world.script.handleDeferedMessages();

    hero.physics.pullLocation();

    for (auto& character : m_game.world.state.characters) {
      character.physics.pullLocation();
    }

    // hero (again)

    hero.aspects.hp.update(time, HPUpdatePeriod);
    hero.aspects.mp.update(time, MPUpdatePeriod);
    hero.aspects.vp.update(time, VPUpdatePeriod);

    auto heroMayAttack = [&]() {
      if (hero.weapon.phase == WeaponPhase::Ready) {
        return;
      }

      // hero wants to attack

      if (!hero.weapon.ref) {
        // hero has no weapon
        gf::Log::debug("NO WEAPON!\n");
        hero.weapon.phase = WeaponPhase::Ready;
        return;
      }

      if (hero.weapon.phase != WeaponPhase::Launch) {
        // hero can not launch an attack for now
        hero.weapon.update(time);
        return;
      }

      // launch an attack!
      // see https://akagoria.github.io/game_system.html#_combat_resolution

      gf::Log::debug("--{ attack from *hero*\n");

      // 1. Check if the attack is valid

      Value aspect = hero.aspects[getAspectFromWeaponType(hero.weapon.ref().type)].value;
      gf::Log::debug("aspect: %" PRIi32 "\n", aspect.asInt());

      if (aspect < hero.weapon.ref().aspect) {
        gf::Log::debug("INVALID! Required aspect: %" PRIi32 "\n", hero.weapon.ref().aspect.asInt());
        gf::Log::debug("--}\n");
        return;
      }

      Value attribute = hero.attributes[getAttributeFromWeaponType(hero.weapon.ref().type)].value;
      gf::Log::debug("attribute: %" PRIi32 "\n", attribute.asInt());

      if (attribute < hero.weapon.ref().attribute) {
        gf::Log::debug("INVALID! Required attribute: %" PRIi32 "\n", hero.weapon.ref().attribute.asInt());
        gf::Log::debug("--}\n");
        return;
      }

      // 2. Compute success of the action

      Value r = m_game.random.computeUniformFloat(0.0f, 100.0f);
      gf::Log::debug("random: %" PRIi32 "\n", r.asInt());

      if (r > attribute) {
        // attack is failed
        gf::Log::debug("FAILED!\n");
        gf::Log::debug("--}\n");
        hero.weapon.phase = WeaponPhase::CoolDown;
        hero.weapon.timer = gf::Time::Zero;
        return;
      }

      Value extent = attribute - r;
      float e = 1.0f + extent.asFloat() / 100.0f;

      // 3. Compute power of the attack

      Value atk = hero.weapon.ref().attack * e * std::sqrt(static_cast<float>(hero.progression.level));
      gf::Log::debug("weapon atk: %" PRIi32 "\n", hero.weapon.ref().attack.asInt());
      gf::Log::debug("extent of success: %.4g\n", e);
      gf::Log::debug("level: %" PRIi32 " (%.4g)\n", hero.progression.level, std::sqrt(static_cast<float>(hero.progression.level)));
      gf::Log::debug("atk: %" PRIi32 "\n", atk.asInt());

      for (auto& character : m_game.world.state.characters) {
        if (squareDistanceToHero(character.physics.location.position) > gf::square(hero.weapon.ref().range)) {
          continue;
        }

        // TODO: check the angle of the weapon

        // 4. Compute power of the defense

        Value def = 3; // TODO: compute the defensive points of the character
        gf::Log::debug("def: %" PRIi32 "\n", def.asInt());

        if (def > atk) {
          gf::Log::debug("MISSED!\n");

          DamageGenerationMessage message;
          message.receiver = DamageReceiver::Other;
          message.position = computeDamagePosition(hero.physics.location.position, character.physics.location.position);
          m_game.messages.sendMessage(&message);

          continue;
        }

        // 5. Compute the damage

        gf::Log::debug("SUCCESS!\n");
        // TODO: compute the damage of the character
        // TODO: update the attribute
      }

      hero.weapon.phase = WeaponPhase::CoolDown;
      hero.weapon.timer = gf::Time::Zero;
      gf::Log::debug("--}\n");
    };

    heroMayAttack();

    // character

    for (auto& character : m_game.world.state.characters) {
      if (character.mood != CharacterMood::Angry) {
        continue;
      }

      if (character.physics.location.floor != floor) {
        continue;
      }

      if (!character.weapon.ref) {
        gf::Log::debug("NO WEAPON!\n");
        continue;
      }

      if (squareDistanceToHero(character.physics.location.position) > gf::square(character.weapon.ref().range)) {
        // TODO: define a vision range + beam
        // gf::Log::debug("DISTANCE: %f vs %f\n", squareDistanceToHero(character.physics.location.position), gf::square(character.weapon.ref().range));

        character.weapon.phase = WeaponPhase::Ready;
        continue;
      }

      switch (character.weapon.phase) {
        case WeaponPhase::Ready:
          character.weapon.phase = WeaponPhase::WarmUp;
          character.weapon.timer = gf::Time::zero();
          break;

        case WeaponPhase::Launch: {
          // see https://akagoria.github.io/game_system.html#_combat_resolution

          gf::Log::debug("--{ attack from character '%s'\n", character.ref().name.tag.c_str());

          // 1. The attack is always considered valid for a character

          Value attribute = character.ref().attribute;
          gf::Log::debug("attribute: %" PRIi32 "\n", attribute.asInt());

          // 2. Compute success of the action

          Value r = m_game.random.computeUniformFloat(0.0f, 100.0f);
          gf::Log::debug("random: %" PRIi32 "\n", r.asInt());

          if (r > attribute) {
            // attack is failed
            gf::Log::debug("FAILED!\n");
            gf::Log::debug("--}\n");
            character.weapon.phase = WeaponPhase::CoolDown;
            character.weapon.timer = gf::Time::Zero;
            continue;
          }

          Value extent = attribute - r;
          float e = 1.0f + extent.asFloat() / 100.0f;

          // 3. Compute power of the attack

          Value atk = character.weapon.ref().attack * e * std::sqrt(static_cast<float>(character.ref().level));
          gf::Log::debug("weapon atk: %" PRIi32 "\n", character.weapon.ref().attack.asInt());
          gf::Log::debug("extent of success: %.4g\n", e);
          gf::Log::debug("level: %" PRIi32 " (%.4g)\n", character.ref().level, std::sqrt(static_cast<float>(character.ref().level)));
          gf::Log::debug("atk: %" PRIi32 "\n", atk.asInt());

          // 4. Compute power of the defense

          Value def = 3; // TODO: compute the defensive points of the hero
          gf::Log::debug("def: %" PRIi32 "\n", def.asInt());

          if (def > atk) {
            gf::Log::debug("MISSED!\n");
            gf::Log::debug("--}\n");
            character.weapon.phase = WeaponPhase::CoolDown;
            character.weapon.timer = gf::Time::Zero;

            DamageGenerationMessage message;
            message.receiver = DamageReceiver::Hero;
            message.position = computeDamagePosition(character.physics.location.position, hero.physics.location.position);
            m_game.messages.sendMessage(&message);

            continue;
          }

          // 5. Compute the damage

          gf::Log::debug("--}\n");
          hero.aspects.hp.value -= character.weapon.ref().attack;

          DamageGenerationMessage message;
          message.receiver = DamageReceiver::Hero;
          message.value = character.weapon.ref().attack;
          message.position = computeDamagePosition(character.physics.location.position, hero.physics.location.position);
          m_game.messages.sendMessage(&message);

          character.weapon.phase = WeaponPhase::CoolDown;
          character.weapon.timer = gf::Time::Zero;
          break;
        }

        default:
          character.weapon.update(time);
          break;
      }

    }

    // shrines

    for (auto& shrine : m_game.world.data.landscape.shrines) {
      if (!isAspectShrine(shrine.type)) {
        continue;
      }

      if (shrine.location.floor != floor) {
        continue;
      }

      if (squareDistanceToHero(shrine.location.position) > gf::square(ShrineDistance)) {
        continue;
      }

      Aspect aspect = getAspectFromShrine(shrine.type);

      if (!hero.aspects[aspect].increase(time)) {
        continue;
      }

      AspectChangeMessage message;
      message.shrine = &shrine;
      message.aspect = aspect;
      message.time = time;
      m_game.messages.sendMessage(&message);
    }


    /*
     * root
     */

    for (auto& character : m_game.world.state.characters) {
      if (!character.dialog) {
        continue;
      }

      if (character.physics.location.floor != hero.physics.location.floor) {
        continue;
      }

      if (squareDistanceToHero(character.physics.location.position) < gf::square(DialogDistance)) {
        m_game.root.scenery.helper.status = HelperStatus::Talk;
      }
    }

    for (auto& item : m_game.world.state.items) {
      if (item.physics.location.floor != hero.physics.location.floor) {
        continue;
      }

      if (squareDistanceToHero(item.physics.location.position) < gf::square(ItemDistance + item.ref().shape.getPhysicalSize())) {
        m_game.root.scenery.helper.status = HelperStatus::Pick;
      }
    }

    /*
     * update view
     */

    setWorldViewCenter(m_game.world.state.hero.physics.location.position);
  } // end of doUpdate

}
