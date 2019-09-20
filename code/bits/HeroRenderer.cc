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
#include <gf/RenderTarget.h>

#include "WorldState.h"

namespace akgr {

  HeroRenderer::HeroRenderer(const WorldState& state, gf::ResourceManager& resources)
  : m_state(state)
  , m_currentAnimation(&m_staticAnimation)
  {
    auto& texture = resources.getTexture("sprites/kalista.png");
    texture.setSmooth(true);

    m_staticAnimation.addFrame(texture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 0.25f, 0.25f }), gf::seconds(1.0f));

    m_forwardAnimation.addFrame(texture, gf::RectF::fromPositionSize({ 0.0f,  0.0f }, { 0.25f, 0.25f }), gf::seconds(0.15f));
    m_forwardAnimation.addFrame(texture, gf::RectF::fromPositionSize({ 0.25f, 0.0f }, { 0.25f, 0.25f }), gf::seconds(0.20f));
    m_forwardAnimation.addFrame(texture, gf::RectF::fromPositionSize({ 0.0f,  0.0f }, { 0.25f, 0.25f }), gf::seconds(0.15f));
    m_forwardAnimation.addFrame(texture, gf::RectF::fromPositionSize({ 0.5f,  0.0f }, { 0.25f, 0.25f }), gf::seconds(0.20f));

    m_backwardAnimation.addFrame(texture, gf::RectF::fromPositionSize({ 0.0f,  0.0f }, { 0.25f, 0.25f }), gf::seconds(0.20f));
    m_backwardAnimation.addFrame(texture, gf::RectF::fromPositionSize({ 0.5f,  0.0f }, { 0.25f, 0.25f }), gf::seconds(0.30f));
    m_backwardAnimation.addFrame(texture, gf::RectF::fromPositionSize({ 0.0f,  0.0f }, { 0.25f, 0.25f }), gf::seconds(0.20f));
    m_backwardAnimation.addFrame(texture, gf::RectF::fromPositionSize({ 0.25f, 0.0f }, { 0.25f, 0.25f }), gf::seconds(0.30f));
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

  void HeroRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::AnimatedSprite sprite;
    sprite.setAnimation(*m_currentAnimation);
    sprite.setPosition(m_state.hero.physics.location.position);
    sprite.setRotation(m_state.hero.physics.angle);
    sprite.setAnchor(gf::Anchor::Center);
    target.draw(sprite, states);
  }

}
