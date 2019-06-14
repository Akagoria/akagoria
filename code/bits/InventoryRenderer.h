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
#ifndef AKGR_INVENTORY_RENDERER_H
#define AKGR_INVENTORY_RENDERER_H

#include <gf/Entity.h>

#include "Display.h"
#include "WorldData.h"
#include "WorldScenery.h"
#include "WorldState.h"
#include "UIData.h"

#include "ui/Scenery.h"
#include "ui/Theme.h"
#include "ui/Widgets.h"

namespace akgr {

  class InventoryModel : public ui::ListModel {
  public:
    InventoryModel(const ui::WidgetListScenery& scenery, const std::vector<InventoryItem>& items);

    InventoryModel(const InventoryModel&) = delete;
    InventoryModel(InventoryModel&&) = default;

    InventoryModel& operator=(const InventoryModel&) = delete;
    InventoryModel& operator=(InventoryModel&&) = default;

    ~InventoryModel();

    virtual void update(ui::Widget* parent) override;
    virtual std::size_t getWidgetCount() override;
    virtual ui::Widget* getWidget(std::size_t i) override;

  private:
    void clear();

  private:
    const ui::WidgetListScenery& m_scenery;
    const std::vector<InventoryItem>& m_items;
    std::vector<ui::Widget*> m_widgets;
  };


  class InventoryRenderer : public gf::Entity {
  public:
    InventoryRenderer(const UIData& ui, const WorldData& data, const WorldState& state, const WorldScenery& scenery, const Display& display, ui::Theme& theme, gf::ResourceManager& resources);

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    const UIData& m_ui;
    const WorldData& m_data;
    const WorldState& m_state;
    const WorldScenery& m_scenery;
    const Display& m_display;
    ui::Theme& m_theme;
    gf::ResourceManager& m_resources;
    InventoryModel m_model;
    ui::FrameWidget m_frame;
  };

}

#endif // AKGR_INVENTORY_RENDERER_H
