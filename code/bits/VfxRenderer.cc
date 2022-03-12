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
#include "VfxRenderer.h"

#include <cassert>

#include <gf/Color.h>
#include <gf/Easings.h>
#include <gf/Particles.h>
#include <gf/RenderTarget.h>
#include <gf/Text.h>

#include "Aspect.h"
#include "WorldMessages.h"

namespace akgr {

  namespace {

    constexpr std::size_t ShrineParticleCount = 20;
    constexpr float ShrineParticleMinRadius = 30.0f;


    constexpr float AspectParticleFadeoutDistance = 25.0f;
    constexpr float AspectParticleVelocity = 250.0f;

    constexpr gf::Time AspectParticleSpawnPeriod = gf::milliseconds(150);


    gf::Color4f getColorFromShrineType(ShrineType type) {
      switch (type) {
        case ShrineType::Ale:
          return gf::Color::Yellow;
        case ShrineType::Ike:
          return gf::Color::White;
        case ShrineType::Moli:
          return gf::Color::Black;
        case ShrineType::Sewi:
          return gf::Color::Orange;
        case ShrineType::Anpa:
          return gf::Color::darker(gf::Color::Magenta);
        case ShrineType::Pona:
          return getAspectColor(Aspect::Health);
        case ShrineType::Wawa:
          return getAspectColor(Aspect::Magic);
        case ShrineType::Sijelo:
          return getAspectColor(Aspect::Vitality);
      }

      assert(false);
      return gf::Color::White;
    }

    constexpr float VfxShrineParticleSize = 3.0f;

  }

  gf::Vector2f VfxShrineParticle::getPosition() const {
    float rho = amplitude * (1.0f + e * std::cos(n * theta));
    return rho * gf::unit(theta);
  }

  gf::Vector2f VfxShrineParticle::getVelocity() const {
    float rho = amplitude * (1.0f + e * std::cos(n * theta));
    float drho = - amplitude * e * std::sin(n * theta) * n * (clockwise ? velocity : - velocity);
    return drho * gf::unit(theta) + rho * velocity * gf::perp(gf::unit(theta));
  }


  VfxRenderer::VfxRenderer(const WorldData& data, const WorldState& state, gf::ResourceManager& resources, gf::MessageManager& messages, gf::Random& random)
  : m_data(data)
  , m_state(state)
  , m_random(random)
  , m_font(resources.getFont("fonts/DejaVuSans.ttf"))
  {
    messages.registerHandler<DamageGenerationMessage>(&VfxRenderer::onDamageGenerationMessage, this);
    messages.registerHandler<AspectChangeMessage>(&VfxRenderer::onAspectChangeMessage, this);

    for (auto& shrine : m_data.landscape.shrines) {
      VfxShrineEmitter emitter;
      emitter.data = &shrine;

      for (std::size_t i = 0; i < ShrineParticleCount; ++i) {
        VfxShrineParticle particle;
        particle.velocity = random.computeUniformFloat(0.5f * gf::Pi, 1.5f * gf::Pi);
        particle.amplitude = ShrineParticleMinRadius;
        particle.theta = random.computeUniformFloat(0.0f, 2 * gf::Pi);
        particle.n = random.computeUniformFloat(1.0f, 3.0f);
        particle.e = random.computeUniformFloat(0.5f, 1.5f);
        particle.clockwise = (i % 2 == 0);
        emitter.particles.push_back(particle);
      }

      shrineEmitters.push_back(std::move(emitter));
    }

  }

  void VfxRenderer::update(gf::Time time) {
    float dt = time.asSeconds();

    for (auto& shrine : shrineEmitters) {
      if (shrine.data->location.floor != m_state.hero.physics.location.floor) {
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
      auto& particles = aspectEmitter.particles;

      for (auto& particle : particles) {
        gf::Vector2f velocity = m_state.hero.physics.location.position - particle.position;
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

    for (auto& damage : damageEmitter.damages) {
      damage.duration -= time;
    }

    damageEmitter.damages.erase(
      std::remove_if(damageEmitter.damages.begin(), damageEmitter.damages.end(), [](const auto& damage) { return damage.duration < gf::Time::zero(); }),
      damageEmitter.damages.end()
    );
  }

  void VfxRenderer::renderFloor(gf::RenderTarget& target, const gf::RenderStates& states, int32_t floor) {
    gf::ShapeParticles particles;

    // shrines

    for (auto& shrine : shrineEmitters) {
      if (shrine.data->location.floor != floor) {
        continue;
      }

      gf::Vector2f center = shrine.data->location.position;

      for (auto& particle : shrine.particles) {
        gf::Vector2f position = center + particle.getPosition();
        particles.addCircle(position, VfxShrineParticleSize, getColorFromShrineType(shrine.data->type));
      }
    }

    // aspect

    for (auto& particle : aspectEmitter.particles) {
      particles.addCircle(particle.position, VfxShrineParticleSize, particle.color);
    }

    target.draw(particles, states);

    // damage

    for (auto& damage : damageEmitter.damages) {
      float alpha = gf::Ease::quartOut(damage.duration.asSeconds() / VfxDamage::Duration.asSeconds());

      gf::Color4f color = damage.receiver == DamageReceiver::Other ? gf::Color::Green : gf::Color::Red;
      gf::Color4f outline = gf::Color::darker(color);

      color.a = outline.a = alpha;

      gf::Text text(damage.message, m_font, 60);
      text.setColor(color);
      text.setOutlineColor(outline);
      text.setOutlineThickness(6.0f);
      text.setPosition(damage.position);
      text.setAnchor(gf::Anchor::Center);
      text.scale(0.5f);
      target.draw(text, states);
    }

  }

  gf::MessageStatus VfxRenderer::onDamageGenerationMessage([[maybe_unused]] gf::Id id, gf::Message *msg) {
    assert(id == DamageGenerationMessage::type);
    auto generation = static_cast<DamageGenerationMessage*>(msg);

    VfxDamage damage;
    damage.receiver = generation->receiver;
    damage.message = std::to_string(generation->value.asInt());
    damage.duration = VfxDamage::Duration;
    damage.position = generation->position;
    damageEmitter.damages.push_back(std::move(damage));

    return gf::MessageStatus::Keep;
  }

  gf::MessageStatus VfxRenderer::onAspectChangeMessage(gf::Id id, gf::Message *msg) {
    assert(id == AspectChangeMessage::type);
    auto change = static_cast<AspectChangeMessage*>(msg);

    for (auto& shrine : shrineEmitters) {
      if (shrine.data != change->shrine) {
        continue;
      }

      shrine.spawnDelay += change->time;

      while (shrine.spawnDelay > AspectParticleSpawnPeriod) {
        assert(!shrine.particles.empty());
        std::size_t index = m_random.computeUniformInteger<std::size_t>(0, shrine.particles.size() - 1);
        auto& spawn = shrine.particles[index];

        VfxAspectParticle particle;
        particle.position = shrine.data->location.position + spawn.getPosition();
        particle.color = getAspectColor(change->aspect);
        particle.alive = true;
        aspectEmitter.particles.push_back(particle);

        shrine.spawnDelay -= AspectParticleSpawnPeriod;
      }

      break;
    }


    return gf::MessageStatus::Keep;
  }

}
