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

    constexpr gf::Time AspectParticleSpawnPeriod = gf::milliseconds(150);
    constexpr float AspectParticleFadeoutDistance = 25.0f;
    constexpr float AspectParticleVelocity = 250.0f;

    bool isAspectShrine(ShrineType shrine) {
      switch (shrine) {
        case ShrineType::Pona:
        case ShrineType::Sewi:
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
        case ShrineType::Sewi:
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
  , m_ground(Plane::Ground, game.world.data, game.world.state, game.world.scenery.map.groundTiles)
  , m_lowTile(Plane::Low, game.world.data, game.world.state, game.world.scenery.map.lowTiles)
  , m_lowSprite(Plane::Low, game.world.data, game.world.state, game.world.scenery.map.lowSprites)
  , m_highTile(Plane::High, game.world.data, game.world.state, game.world.scenery.map.highTiles)
  , m_highSprite(Plane::High, game.world.data, game.world.state, game.world.scenery.map.highSprites)
  , m_hero(game.world.state, game.resources)
  , m_character(game.world.data, game.world.state)
  , m_item(game.world.data, game.world.state, game.resources)
  , m_vfx(game.world.scenery, game.world.state, game.resources)
  , m_helper(game.root.data, game.root.scenery, game.resources)
  , m_debug(game.world.state.physics.model)
  , m_listener(game.world.script)
  {
    addWorldEntity(m_ground);
    addWorldEntity(m_lowTile);
    addWorldEntity(m_lowSprite);
    addWorldEntity(m_highTile);
    addWorldEntity(m_highSprite);
    addWorldEntity(m_hero);
    addWorldEntity(m_character);
    addWorldEntity(m_item);
    addWorldEntity(m_vfx);
    addWorldEntity(m_debug);

    addHudEntity(m_helper);

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
      Slot& slot = m_game.root.scenery.selector.games[SlotSelectorScenery::SlotCount - 1];
      m_game.world.state.saveToFile(slot.path);
      slot.active = true;

      slot.meta.area = m_game.world.scenery.area.current->name;
      slot.save();

      gf::Log::debug("Quick save\n");

      m_game.root.scenery.selector.load();
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
    {
      static constexpr float Hop = 150.0f;
      static constexpr float Turn = 3.0f;

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

      if (hero.weapon.ref.id == gf::InvalidId) {
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

      Value aspect = hero.aspects[getAspectFromWeaponType(hero.weapon.ref.data->type)].value;
      gf::Log::debug("aspect: %" PRIi32 "\n", aspect.asInt());

      if (aspect < hero.weapon.ref.data->aspect) {
        gf::Log::debug("INVALID! Required aspect: %" PRIi32 "\n", hero.weapon.ref.data->aspect.asInt());
        gf::Log::debug("--}\n");
        return;
      }

      Value attribute = hero.attributes[getAttributeFromWeaponType(hero.weapon.ref.data->type)].value;
      gf::Log::debug("attribute: %" PRIi32 "\n", attribute.asInt());

      if (attribute < hero.weapon.ref.data->attribute) {
        gf::Log::debug("INVALID! Required attribute: %" PRIi32 "\n", hero.weapon.ref.data->attribute.asInt());
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

      Value atk = hero.weapon.ref.data->attack * e * std::sqrt(static_cast<float>(hero.progression.level));
      gf::Log::debug("weapon atk: %" PRIi32 "\n", hero.weapon.ref.data->attack.asInt());
      gf::Log::debug("extent of success: %.4g\n", e);
      gf::Log::debug("level: %" PRIi32 " (%.4g)\n", hero.progression.level, std::sqrt(static_cast<float>(hero.progression.level)));
      gf::Log::debug("atk: %" PRIi32 "\n", atk.asInt());

      for (auto& character : m_game.world.state.characters) {
        if (squareDistanceToHero(character.physics.location.position) > gf::square(hero.weapon.ref.data->range)) {
          continue;
        }

        // TODO: check the angle of the weapon

        // 4. Compute power of the defense

        Value def = 3; // TODO: compute the defensive points of the character
        gf::Log::debug("def: %" PRIi32 "\n", def.asInt());

        if (def > atk) {
          gf::Log::debug("MISSED!\n");

          VfxDamage damage;
          damage.receiver = VfxDamageReceiver::Other;
          damage.message = "0";
          damage.duration = VfxDamage::Duration;
          damage.position = computeDamagePosition(hero.physics.location.position, character.physics.location.position);
          m_game.world.scenery.vfx.damageEmitter.damages.push_back(std::move(damage));
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

      if (character.weapon.ref.id == gf::InvalidId) {
        gf::Log::debug("NO WEAPON!\n");
        continue;
      }

      if (squareDistanceToHero(character.physics.location.position) > gf::square(character.weapon.ref.data->range)) {
        // TODO: define a vision range + beam
        // gf::Log::debug("DISTANCE: %f vs %f\n", squareDistanceToHero(character.physics.location.position), gf::square(character.weapon.ref.data->range));

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

          gf::Log::debug("--{ attack from character '%s'\n", character.ref.data->name.c_str());

          // 1. The attack is always considered valid for a character

          Value attribute = character.ref.data->attribute;
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

          Value atk = character.weapon.ref.data->attack * e * std::sqrt(static_cast<float>(character.ref.data->level));
          gf::Log::debug("weapon atk: %" PRIi32 "\n", character.weapon.ref.data->attack.asInt());
          gf::Log::debug("extent of success: %.4g\n", e);
          gf::Log::debug("level: %" PRIi32 " (%.4g)\n", character.ref.data->level, std::sqrt(static_cast<float>(character.ref.data->level)));
          gf::Log::debug("atk: %" PRIi32 "\n", atk.asInt());

          // 4. Compute power of the defense

          Value def = 3; // TODO: compute the defensive points of the hero
          gf::Log::debug("def: %" PRIi32 "\n", def.asInt());

          if (def > atk) {
            gf::Log::debug("MISSED!\n");
            gf::Log::debug("--}\n");
            character.weapon.phase = WeaponPhase::CoolDown;
            character.weapon.timer = gf::Time::Zero;

            VfxDamage damage;
            damage.receiver = VfxDamageReceiver::Hero;
            damage.message = "0";
            damage.duration = VfxDamage::Duration;
            damage.position = computeDamagePosition(character.physics.location.position, hero.physics.location.position);
            m_game.world.scenery.vfx.damageEmitter.damages.push_back(std::move(damage));

            continue;
          }

          // 5. Compute the damage

          gf::Log::debug("--}\n");
          hero.aspects.hp.value -= character.weapon.ref.data->attack;

          VfxDamage damage;
          damage.receiver = VfxDamageReceiver::Hero;
          damage.message = std::to_string(character.weapon.ref.data->attack.asInt());
          damage.duration = VfxDamage::Duration;
          damage.position = computeDamagePosition(character.physics.location.position, hero.physics.location.position);
          m_game.world.scenery.vfx.damageEmitter.damages.push_back(std::move(damage));

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

    for (auto& shrine : m_game.world.scenery.vfx.shrineEmitters) {
      if (!isAspectShrine(shrine.data->type)) {
        continue;
      }

      if (shrine.data->location.floor != floor) {
        continue;
      }

      if (squareDistanceToHero(shrine.data->location.position) > gf::square(ShrineDistance)) {
        continue;
      }

      Aspect aspect = getAspectFromShrine(shrine.data->type);

      if (!hero.aspects[aspect].increase(time)) {
        continue;
      }

      shrine.spawnDelay += time;

      while (shrine.spawnDelay > AspectParticleSpawnPeriod) {
        assert(!shrine.particles.empty());
        std::size_t index = m_game.random.computeUniformInteger<std::size_t>(0, shrine.particles.size() - 1);
        auto& spawn = shrine.particles[index];

        VfxAspectParticle particle;
        particle.position = shrine.data->location.position + spawn.getPosition();
        particle.color = getAspectColor(aspect);
        particle.alive = true;
        m_game.world.scenery.vfx.aspectEmitter.particles.push_back(particle);

        shrine.spawnDelay -= AspectParticleSpawnPeriod;
      }
    }



    /*
     * scenery
     */

    // vfx

    for (auto& shrine : m_game.world.scenery.vfx.shrineEmitters) {
      if (shrine.data->location.floor != floor) {
        continue;
      }

      for (auto& particle : shrine.particles) {
        if (particle.clockwise) {
          particle.theta += particle.velocity * dt;
        } else {
          particle.theta -= particle.velocity * dt;
        }
      }
    }

    {
      auto& particles = m_game.world.scenery.vfx.aspectEmitter.particles;

      for (auto& particle : particles) {
        gf::Vector2f velocity = hero.physics.location.position - particle.position;
        float distance = gf::euclideanLength(velocity);
        particle.position += AspectParticleVelocity * velocity / distance * dt;

        if (distance < AspectParticleFadeoutDistance) {
          particle.alive = false;
          continue;
        }
      }

      particles.erase(
          std::remove_if(particles.begin(), particles.end(), [](const auto& particle) { return !particle.alive; }),
          particles.end()
      );
    }

    for (auto& damage : m_game.world.scenery.vfx.damageEmitter.damages) {
      damage.duration -= time;
    }

    m_game.world.scenery.vfx.damageEmitter.damages.erase(
      std::remove_if(m_game.world.scenery.vfx.damageEmitter.damages.begin(), m_game.world.scenery.vfx.damageEmitter.damages.end(), [](const auto& damage) { return damage.duration < gf::Time::zero(); }),
      m_game.world.scenery.vfx.damageEmitter.damages.end()
    );

    /*
     * root
     */

    for (auto& character : m_game.world.state.characters) {
      if (character.dialog.id == gf::InvalidId) {
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

      if (squareDistanceToHero(item.physics.location.position) < gf::square(ItemDistance + item.ref.data->shape.getPhysicalSize())) {
        m_game.root.scenery.helper.status = HelperStatus::Pick;
      }
    }

    /*
     * update view
     */

    setWorldViewCenter(m_game.world.state.hero.physics.location.position);
  } // end of doUpdate

}
