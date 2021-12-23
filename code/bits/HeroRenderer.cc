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
#include "HeroRenderer.h"

#include <gf/AnimatedSprite.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>

#include "WorldData.h"
#include "WorldState.h"

namespace akgr {

  namespace {
    gf::Animation loadAnimation(const WorldData& data, gf::ResourceManager& resources, std::string name) {
      gf::Animation animation;

      auto it = data.hero.animations.find(gf::hash(name));

      if (it == data.hero.animations.end()) {
        gf::Log::error("Unkown animation for hero: '%s'\n", name.c_str());
        return animation;
      }

      const AtlasAnimation& atlasAnimation = it->second;

      for (auto & frame : atlasAnimation.frames) {
        auto it = data.atlases.find(frame.atlas);

        if (it == data.atlases.end()) {
          gf::Log::error("Unknown atlas for animation (hero): '%s'\n", name.c_str());
          continue;
        }

        const AtlasData& atlas = it->second;
        gf::Texture& texture = resources.getTexture(atlas.path);
        texture.setSmooth(true);

        gf::Vector2f textureSize = 1.0f / atlas.size;
        gf::Vector2i textureIndex = { frame.index % atlas.size.width, frame.index / atlas.size.width };

        animation.addFrame(texture, gf::RectF::fromPositionSize(textureSize * textureIndex, textureSize), gf::milliseconds(frame.duration));
      }

      return animation;
    }


  }

  HeroRenderer::HeroRenderer(const WorldData& data, const WorldState& state, gf::ResourceManager& resources)
  : m_state(state)
  , m_staticAnimation(loadAnimation(data, resources, "static"))
  , m_forwardAnimation(loadAnimation(data, resources, "forward"))
  , m_backwardAnimation(loadAnimation(data, resources, "backward"))
  , m_currentAnimation(&m_staticAnimation)
  {
  }

  void HeroRenderer::update(gf::Time time) {
     switch (m_state.hero.move.linear) {
       case gf::LinearMove::None:
         m_currentAnimation = &m_staticAnimation;
         break;

       case gf::LinearMove::Forward:
         m_currentAnimation = &m_forwardAnimation;
         break;

       case gf::LinearMove::Backward:
         m_currentAnimation = &m_backwardAnimation;
         break;
     }

     assert(m_currentAnimation);
     m_currentAnimation->update(time);
  }

  void HeroRenderer::renderFloor(gf::RenderTarget& target, const gf::RenderStates& states, int32_t floor) {
    if (floor != m_state.hero.physics.location.floor) {
      return;
    }

    gf::AnimatedSprite sprite;
    sprite.setAnimation(*m_currentAnimation);
    sprite.setPosition(m_state.hero.physics.location.position);
    sprite.setRotation(m_state.hero.physics.angle);
    sprite.setAnchor(gf::Anchor::Center);
    target.draw(sprite, states);
  }

}
