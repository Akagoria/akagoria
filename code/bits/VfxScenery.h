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
#ifndef AKGR_VFX_SCENERY_H
#define AKGR_VFX_SCENERY_H

#include <cmath>
#include <vector>

#include <gf/Random.h>
#include <gf/Time.h>
#include <gf/Vector.h>
#include <gf/VectorOps.h>

#include "Aspect.h"
#include "LandscapeData.h"

namespace akgr {

  struct VfxShrineParticle {
    float velocity;
    float amplitude;
    float theta;
    float n;
    float e;
    bool clockwise;

    gf::Vector2f getPosition() const {
      float rho = amplitude * (1.0f + e * std::cos(n * theta));
      return rho * gf::unit(theta);
    }

    gf::Vector2f getVelocity() const {
      float rho = amplitude * (1.0f + e * std::cos(n * theta));
      float drho = - amplitude * e * std::sin(n * theta) * n * (clockwise ? velocity : - velocity);
      return drho * gf::unit(theta) + rho * velocity * gf::perp(gf::unit(theta));
    }
  };

  struct VfxShrineEmitter {
    const LandscapeShrineData *data;
    std::vector<VfxShrineParticle> particles;
    gf::Time spawnDelay;
  };


  struct VfxAspectParticle {
    gf::Vector2f position;
    gf::Color4f color;
    bool alive;
  };

  struct VfxAspectEmitter {
    std::vector<VfxAspectParticle> particles;
  };

  enum class VfxDamageReceiver {
    Hero,
    Other,
  };

  struct VfxDamage {
    VfxDamageReceiver receiver;
    std::string message;
    gf::Time duration;
    gf::Vector2f position;

    static constexpr gf::Time Duration = gf::milliseconds(800);
  };

  struct VfxDamageEmitter {
    std::vector<VfxDamage> damages;
  };

  struct VfxScenery {
    VfxAspectEmitter aspectEmitter;
    std::vector<VfxShrineEmitter> shrineEmitters;
    VfxDamageEmitter damageEmitter;
  };

}

#endif // AKGR_VFX_SCENERY_H
