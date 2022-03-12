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
#ifndef AKGR_AREA_RENDERER_H
#define AKGR_AREA_RENDERER_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>

#include "WorldData.h"
#include "WorldState.h"

namespace akgr {

  class AreaRenderer : public gf::Entity {
  public:
    AreaRenderer(gf::ResourceManager& resources, const WorldData& data, const WorldState& state);

    virtual void update(gf::Time time) override;
    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    gf::Font& m_font;
    const WorldData& m_data;
    const WorldState& m_state;

    const AreaData *m_current = nullptr;
    gf::Time m_period = gf::Time::zero();
  };

}

#endif // AKGR_AREA_RENDERER_H
