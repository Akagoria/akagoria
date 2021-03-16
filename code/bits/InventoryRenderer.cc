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
#include "InventoryRenderer.h"

#include <boost/locale.hpp>

#include <gf/Coordinates.h>
#include <gf/VectorOps.h>
#include <gf/Log.h>

#include "Menu.h"

#include "ui/Widgets.h"
#include "ui/Common.h"

using namespace gf::literals;

namespace akgr {

  InventoryModel::InventoryModel(const ui::WidgetListScenery& scenery, const std::vector<InventoryItem>& items)
  : m_scenery(scenery)
  , m_items(items)
  {

  }

  InventoryModel::~InventoryModel() {
    clear();
  }

  void InventoryModel::update(ui::Widget* parent) {
    assert(m_scenery.count == m_items.size());
    assert(m_scenery.choice < m_scenery.count);

    clear();

    if (m_scenery.count == 0) {
      return;
    }

    auto newInventoryItemWidget = [&](const InventoryItem& item) {
      auto widget = new ui::DoubleTextWidget(parent, boost::locale::gettext(item.ref().description.c_str()), std::to_string(item.count));
      widget->setSize(ui::Common::DefaultCaptionSize);
      return widget;
    };

    uint32_t page = m_scenery.choice / m_scenery.itemsPerPage;
    uint32_t start = page * m_scenery.itemsPerPage;
    uint32_t end = std::min(start + m_scenery.itemsPerPage, m_scenery.count);

    for (uint32_t i = start; i < end; ++i) {
      assert(i < m_scenery.count);
      m_widgets.push_back(newInventoryItemWidget(m_items[i]));
    }
  }

  std::size_t InventoryModel::getWidgetCount() {
    return m_widgets.size();
  }

  ui::Widget* InventoryModel::getWidget(std::size_t i) {
    assert(i < m_widgets.size());
    return m_widgets[i];
  }

  void InventoryModel::clear() {
    for (auto widget : m_widgets) {
      delete widget;
    }

    m_widgets.clear();
  }


  InventoryRenderer::InventoryRenderer(const RootData& root, const WorldData& data, const WorldState& state, const WorldScenery& scenery, ui::Theme& theme, gf::ResourceManager& resources)
  : gf::Entity(10)
  , m_root(root)
  , m_data(data)
  , m_state(state)
  , m_scenery(scenery)
  , m_theme(theme)
  , m_resources(resources)
  , m_model(scenery.inventory.list, state.hero.inventory.items)
  , m_frame(nullptr)
  {
    m_frame.add<ui::CatalogueWidget>(m_model, scenery.inventory.list);

    m_frame.setPosition(ui::Common::Position);
    m_frame.setSize({ 1.0f, 0.95f });
    m_frame.setPositioning(ui::Positioning::Minimum);
  }

  void InventoryRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    m_frame.computeLayout();
    m_frame.render(target, states, m_theme);
  }

}
