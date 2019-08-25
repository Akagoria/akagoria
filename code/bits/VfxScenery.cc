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
#include "VfxScenery.h"

#include <gf/Color.h>
#include <gf/VectorOps.h>

namespace akgr {

  void VfxScenery::onAspectBoost(gf::Random& random, Aspect aspect) {
    static constexpr int Count = 50;

    static constexpr float DelayMin = 0.0f;
    static constexpr float DelayMax = 0.8f;
    static constexpr float DistanceMin = 20.0f;
    static constexpr float DistanceMax = 80.0f;

    for (int i = 0; i < Count; ++i) {
      float delay = random.computeUniformFloat(DelayMin, DelayMax);
      float angle = random.computeAngle();
      float distance = random.computeRadius(DistanceMin, DistanceMax);

      VfxAspectParticle particle;
      particle.delay = gf::seconds(delay);
      particle.angle = angle;
      particle.distance = distance;
      particle.color = gf::Color::lighter(getAspectColor(aspect), 0.75f);
      particle.lifetime = gf::seconds(VfxAspectParticle::Lifetime);

      aspectEmitter.particles.push_back(particle);
    }
  }

}
