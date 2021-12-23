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
#ifndef AKGR_FLOOR_RENDERER_H
#define AKGR_FLOOR_RENDERER_H

#include <vector>

#include <gf/Ref.h>
#include <gf/RenderStates.h>
#include <gf/RenderTarget.h>
#include <gf/Time.h>

namespace akgr {


  class FloorRenderer {
  public:
    FloorRenderer(int priority = 0)
    : m_priority(priority)
    {
    }

    virtual ~FloorRenderer();

    int getPriority() const noexcept {
      return m_priority;
    }

    virtual void update(gf::Time time);
    virtual void renderFloor(gf::RenderTarget& target, const gf::RenderStates& states, int32_t floor) = 0;

  private:
    int m_priority;
  };

  class FloorRendererContainer {
  public:
    void addFloorRenderer(FloorRenderer& renderer);

    void update(gf::Time time);
    void renderFloor(gf::RenderTarget& target, const gf::RenderStates& states, int32_t floor);

  private:
    std::vector<gf::Ref<FloorRenderer>> m_renderers;
  };

}


#endif // AKGR_FLOOR_RENDERER_H
