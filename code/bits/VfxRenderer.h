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

#include <gf/ResourceManager.h>

#include "FloorRenderer.h"
#include "WorldScenery.h"
#include "WorldState.h"

namespace akgr {

  class VfxRenderer : public FloorRenderer {
  public:
    VfxRenderer(const WorldScenery& scenery, gf::ResourceManager& resources);

    void renderFloor(gf::RenderTarget& target, const gf::RenderStates& states, int32_t floor) override;

  private:
    const WorldScenery& m_scenery;
    gf::Font& m_font;
  };
}

#endif // AKGR_VFX_RENDERER_H
