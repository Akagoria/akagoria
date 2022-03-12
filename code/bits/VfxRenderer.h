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
#ifndef AKGR_VFX_RENDERER_H
#define AKGR_VFX_RENDERER_H

#include <string>
#include <vector>

#include <gf/MessageManager.h>
#include <gf/ResourceManager.h>
#include <gf/Time.h>
#include <gf/Vector.h>

#include "FloorRenderer.h"
#include "WorldData.h"
#include "WorldMessages.h"
#include "WorldScenery.h"
#include "WorldState.h"

namespace akgr {

  struct VfxShrineParticle {
    float velocity;
    float amplitude;
    float theta;
    float n;
    float e;
    bool clockwise;

    gf::Vector2f getPosition() const;
    gf::Vector2f getVelocity() const;
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

  struct VfxDamage {
    DamageReceiver receiver;
    std::string message;
    gf::Time duration;
    gf::Vector2f position;

    static constexpr gf::Time Duration = gf::milliseconds(800);
  };

  struct VfxDamageEmitter {
    std::vector<VfxDamage> damages;
  };

  class VfxRenderer : public FloorRenderer {
  public:
    VfxRenderer(const WorldData& data, const WorldState& state, gf::ResourceManager& resources, gf::MessageManager& messages, gf::Random& random);

    void update(gf::Time time) override;
    void renderFloor(gf::RenderTarget& target, const gf::RenderStates& states, int32_t floor) override;

  private:
    gf::MessageStatus onDamageGenerationMessage(gf::Id id, gf::Message *msg);
    gf::MessageStatus onAspectChangeMessage(gf::Id id, gf::Message *msg);

  private:
    const WorldData& m_data;
    const WorldState& m_state;
    gf::Random& m_random;
    gf::Font& m_font;

    VfxAspectEmitter aspectEmitter;
    std::vector<VfxShrineEmitter> shrineEmitters;
    VfxDamageEmitter damageEmitter;
  };
}

#endif // AKGR_VFX_RENDERER_H
