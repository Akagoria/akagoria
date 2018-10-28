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
#include "WorldScenery.h"

#include "WorldData.h"

namespace akgr {

  namespace {

    constexpr std::size_t ShrineParticleCount = 20;
    constexpr float ShrineParticleMinRadius = 30.0f;


  }

  void WorldScenery::bind(const WorldData& data, gf::Random& random) {
    // shrines

    for (auto& shrine : data.shrines) {
      ShrineScenery scenery;
      scenery.data = &shrine;

      for (std::size_t i = 0; i < ShrineParticleCount; ++i) {
        ShrineParticle particle;
        particle.velocity = random.computeUniformFloat(0.5 * gf::Pi, 1.5 * gf::Pi);
        particle.amplitude = ShrineParticleMinRadius;
        particle.theta = random.computeUniformFloat(0.0f, 2 * gf::Pi);
        particle.n = random.computeUniformFloat(1.0f, 3.0f);
        particle.e = random.computeUniformFloat(0.5f, 1.5f);
        particle.clockwise = (i % 2 == 0);
        scenery.particles.push_back(particle);
      }

      shrines.push_back(std::move(scenery));
    }

  }



}
