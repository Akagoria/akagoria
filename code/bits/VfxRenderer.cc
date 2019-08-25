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

#include "Aspect.h"

namespace akgr {

  namespace {

    gf::Color4f getColorFromShrineType(ShrineType type) {
      switch (type) {
        case ShrineType::Ale:
          return gf::Color::Yellow;
        case ShrineType::Ike:
          return gf::Color::White;
        case ShrineType::Moli:
          return gf::Color::Black;
        case ShrineType::Pona:
          return getAspectColor(Aspect::Health);
        case ShrineType::Sewi:
          return getAspectColor(Aspect::Magic);
        case ShrineType::Sijelo:
          return getAspectColor(Aspect::Vitality);
      }

      assert(false);
      return gf::Color::White;
    }

    const float VfxShrineParticleSize = 2.0f;

  }

  VfxRenderer::VfxRenderer(const WorldScenery& scenery, const WorldState& state)
  : m_scenery(scenery)
  , m_state(state)
  {

  }

  void VfxRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Vector2f hero = m_state.hero.physics.location.position;

    gf::ShapeParticles particles;

    // shrines

    auto floor = m_state.hero.physics.location.floor;

    for (auto& shrine : m_scenery.vfx.shrineEmitters) {
      if (shrine.data->location.floor != floor) {
        continue;
      }

      gf::Vector2f center = shrine.data->location.position;

      for (auto& particle : shrine.particles) {
        float rho = particle.amplitude * (1.0f + particle.e * std::cos(particle.n * particle.theta));
        gf::Vector2f position = center + rho * gf::unit(particle.theta);

        particles.addCircle(position, VfxShrineParticleSize, getColorFromShrineType(shrine.data->type));
      }
    }


    // aspect

    for (auto& particle : m_scenery.vfx.aspectEmitter.particles) {
      static constexpr float RadiusMin = 0.8f;
      static constexpr float RadiusMax = 2.0f;

      if (particle.delay > gf::Time::zero()) {
        continue;
      }

      float t = particle.lifetime.asSeconds() / VfxAspectParticle::Lifetime;
      float distance = gf::Ease::cubicOut(t) * particle.distance;
      float radius = gf::lerp(RadiusMin, RadiusMax, t);

      particles.addCircle(hero + distance * gf::unit(particle.angle), radius, particle.color);
    }

    target.draw(particles, states);
  }

}
