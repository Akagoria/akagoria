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

#include <gf/Coordinates.h>
#include <gf/VectorOps.h>

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
    std::size_t count = m_items.size();

    clear();
    std::size_t visible = std::min(count, m_scenery.length);

    if (visible == 0) {
      return;
    }

    auto newInventoryItemWidget = [&](const InventoryItem& item) {
      auto widget = new ui::DoubleTextWidget(parent, item.ref.data->description, std::to_string(item.count));
      widget->setSize(ui::Common::DefaultCaptionSize);
      return widget;
    };

    auto newDotsWidget = [&]() {
      auto widget = new ui::LabelWidget(parent, "...");
      widget->setSize(ui::Common::DefaultCaptionSize);
      return widget;
    };

    if (m_scenery.start > 0) {
      m_widgets.push_back(newDotsWidget());
    } else {
      m_widgets.push_back(newInventoryItemWidget(m_items.front()));
    }

    for (std::size_t i = 1; i < visible - 1; ++i) {
      assert(m_scenery.start + i < count);
      m_widgets.push_back(newInventoryItemWidget(m_items[m_scenery.start + i]));
    }

    if (count > m_scenery.length) {
      if (m_scenery.start + m_scenery.length < count) {
        m_widgets.push_back(newDotsWidget());
      } else {
        assert(visible == m_scenery.length);
        m_widgets.push_back(newInventoryItemWidget(m_items.back()));
      }
    } else {
      assert(visible == count);
      m_widgets.push_back(newInventoryItemWidget(m_items.back()));
    }

    assert(m_widgets.size() == visible);
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


  InventoryRenderer::InventoryRenderer(const UIData& ui, const WorldData& data, const WorldState& state, const WorldScenery& scenery, ui::Theme& theme, gf::ResourceManager& resources)
  : gf::Entity(10)
  , m_ui(ui)
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
    m_frame.computeLayout();
  }

  void InventoryRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    if (m_state.operation != WorldOperation::Inventory) {
      return;
    }

    m_frame.computeLayout();
    m_frame.render(target, states, m_theme);

#if 0

    gf::Coordinates coords(target);

    gf::Vector2f previewPosition = coords.getRelativePoint(InventoryPreviewPosition);
    float previewSize = coords.getRelativeSize(InventoryPreviewSize).width;

    assert(m_scenery.inventory.current < m_scenery.inventory.size);
    const InventoryItem& item =  m_state.hero.inventory.items[m_scenery.inventory.current];

    const ItemData& data = *item.ref.data;

    {
      auto it = m_data.catalogue.resources.find(data.graphics.resource);

      if (it == m_data.catalogue.resources.end()) {
        gf::Log::error("Unknown graphics resource for item '%s'\n", data.name.c_str());
        return;
      }

      const ItemResource& resource = it->second;
      const gf::Texture& texture = m_resources.getTexture(resource.path);

      gf::Vector2f textureSize = 1.0f / resource.size;
      gf::Vector2i textureIndex = { data.graphics.index % resource.size.width, data.graphics.index / resource.size.width };

      gf::Vector2f size = texture.getSize() * textureSize;

      gf::Sprite sprite(texture);
      sprite.setTextureRect({ textureSize * textureIndex, textureSize });
      sprite.setAnchor(gf::Anchor::TopRight);
      sprite.setPosition(previewPosition);
      sprite.setScale(previewSize / size);
      target.draw(sprite, states);
    }

#endif

  }

}
