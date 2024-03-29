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
#include "UniverseRenderer.h"

namespace akgr {

  UniverseRenderer::UniverseRenderer(const WorldData& data, const WorldState& state, gf::ResourceManager& resources, gf::MessageManager& messages, gf::Random& random)
  : m_state(state)
  , m_ground(Plane::Ground, data, resources)
  , m_lowTile(Plane::Low, data, resources)
  , m_lowSprite(Plane::Low, data, resources)
  , m_highTile(Plane::High, data, resources)
  , m_highSprite(Plane::High, data, resources)
  , m_hero(data, state, resources)
  , m_character(data, state)
  , m_item(data, state, resources)
  , m_vfx(data, state, resources, messages, random)
  {
    m_renderers.addFloorRenderer(m_ground);
    m_renderers.addFloorRenderer(m_lowTile);
    m_renderers.addFloorRenderer(m_lowSprite);
    m_renderers.addFloorRenderer(m_highTile);
    m_renderers.addFloorRenderer(m_highSprite);
    m_renderers.addFloorRenderer(m_hero);
    m_renderers.addFloorRenderer(m_character);
    m_renderers.addFloorRenderer(m_item);
    m_renderers.addFloorRenderer(m_vfx);
  }

  void UniverseRenderer::update(gf::Time time) {
    m_renderers.update(time);
  }

  void UniverseRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    auto floor = m_state.hero.physics.location.floor;
    m_renderers.renderFloor(target, states, floor);
  }

}
