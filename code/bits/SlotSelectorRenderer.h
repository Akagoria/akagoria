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
#ifndef AKGR_SLOT_SELECTOR_RENDERER_H
#define AKGR_SLOT_SELECTOR_RENDERER_H

#include <gf/Entity.h>

#include "Display.h"
#include "OpeningScenery.h"
#include "SlotSelectorScenery.h"
#include "UIData.h"
#include "WorldScenery.h"
#include "WorldState.h"

namespace akgr {

  class SlotSelectorRenderer : public gf::Entity {
  public:
    SlotSelectorRenderer(const UIData& data, const SlotSelectorScenery& scenery, const Display& display);
    virtual ~SlotSelectorRenderer();

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

    virtual bool isDisplayed() const = 0;

  private:
    const UIData& m_data;
    const SlotSelectorScenery& m_scenery;
    const Display& m_display;
  };

  class OpeningSlotSelectorRenderer : public SlotSelectorRenderer {
  public:
    OpeningSlotSelectorRenderer(const UIData& data, const OpeningScenery& scenery, const Display& display);

    virtual bool isDisplayed() const override;

  private:
    const OpeningScenery& m_scenery;
  };

  class WorldSlotSelectorRenderer : public SlotSelectorRenderer {
  public:
    WorldSlotSelectorRenderer(const UIData& data, const WorldState& state, const WorldScenery& scenery, const Display& display);

    virtual bool isDisplayed() const override;

  private:
    const WorldState& m_state;
  };

}

#endif // AKGR_SLOT_SELECTOR_RENDERER_H
