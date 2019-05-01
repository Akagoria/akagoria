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

using namespace gf::literals;

namespace akgr {

  namespace {

    constexpr gf::Vector2f InventoryPosition = Menu::Position;
    constexpr gf::Vector2f InventorySize = { 0.496f, 0.946f };

    constexpr gf::Vector2f InventoryItemRelPosition = { Menu::ItemRelPosition.x, 0.005f };
    constexpr gf::Vector2f InventoryItemSize = { 0.45f, Menu::ItemSize.y };

    constexpr gf::Vector2f InventoryItemPosition(std::size_t i) {
      gf::Vector2f pos = InventoryPosition + InventoryItemRelPosition;
      pos.y += i * InventoryItemSize.y;
      return pos;
    }

    constexpr gf::Vector2f InventoryArrowRelPosition = { Menu::ArrowRelPosition.x, InventoryItemSize.y / 2 };

    constexpr gf::Vector2f InventoryArrowPosition(std::size_t i) {
      gf::Vector2f pos = InventoryPosition + InventoryArrowRelPosition;
      pos.y += i * InventoryItemSize.y;
      return pos;
    }

    static_assert(InventoryScenery::Length == static_cast<std::size_t>(InventorySize.y / InventoryItemSize.y), "Bad inventory length");


    constexpr gf::Vector2f InventoryPreviewPosition = { 0.95f, 0.03f };
    constexpr gf::Vector2f InventoryPreviewSize = { 0.05f, 0.0f };

  }

  InventoryRenderer::InventoryRenderer(const UIData& ui, const WorldData& data, const WorldState& state, const WorldScenery& scenery, const Display& display, gf::ResourceManager& resources)
  : gf::Entity(10)
  , m_ui(ui)
  , m_data(data)
  , m_state(state)
  , m_scenery(scenery)
  , m_display(display)
  , m_resources(resources)
  {

  }

  void InventoryRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    if (m_state.operation != WorldOperation::Inventory) {
      return;
    }

    // left pane

    assert(m_scenery.inventory.size == m_state.hero.inventory.items.size());

    m_display.renderBox(target, states, { InventoryPosition, InventorySize });
    m_display.renderBox(target, states, { InventoryPosition + gf::dirx(0.5f), InventorySize });

    if (m_scenery.inventory.size == 0) {
      return;
    }

    auto displayItem = [&](const InventoryItem& item, std::size_t i) {
      m_display.renderString(target, states, { InventoryItemPosition(i), InventoryItemSize }, Menu::CharacterSize, item.ref.data->description);
      m_display.renderString(target, states, { InventoryItemPosition(i), InventoryItemSize }, Menu::CharacterSize, std::to_string(item.count), gf::Alignment::Right);
    };

    if (m_scenery.inventory.start > 0) {
      m_display.renderString(target, states, { InventoryItemPosition(0), InventoryItemSize }, Menu::CharacterSize, "...");
    } else {
      displayItem(m_state.hero.inventory.items.front(), 0);
    }

    for (std::size_t i = 1; i < std::min(InventoryScenery::Length, m_scenery.inventory.size) - 1; ++i) {
      assert(m_scenery.inventory.start + i < m_scenery.inventory.size);
      const auto& item = m_state.hero.inventory.items[m_scenery.inventory.start + i];
      displayItem(item, i);
    }

    if (m_scenery.inventory.size > InventoryScenery::Length) {
      if (m_scenery.inventory.start + InventoryScenery::Length < m_scenery.inventory.size) {
        m_display.renderString(target, states, { InventoryItemPosition(InventoryScenery::Length - 1), InventoryItemSize }, Menu::CharacterSize, "...");
      } else {
        displayItem(m_state.hero.inventory.items.back(), InventoryScenery::Length - 1);
      }
    } else {
      displayItem(m_state.hero.inventory.items.back(), m_scenery.inventory.size - 1);
    }

    m_display.renderArrow(target, states, InventoryArrowPosition(m_scenery.inventory.current - m_scenery.inventory.start));

    // right pane

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




    if (m_scenery.inventory.choice == InventoryScenery::NoChoice) {
      return;
    }

//     m_display.renderBox(target, states, { Menu::Position, Menu::TotalSize(GameMenuScenery::ItemCount) });
//     m_display.renderString(target, states, { Menu::ItemPosition(GameMenuScenery::Inventory), Menu::ItemSize }, Menu::CharacterSize, m_data.getUIMessage("MenuInventory"_id));
//     m_display.renderString(target, states, { Menu::ItemPosition(GameMenuScenery::Quests), Menu::ItemSize }, Menu::CharacterSize, m_data.getUIMessage("MenuQuests"_id));
//     m_display.renderString(target, states, { Menu::ItemPosition(GameMenuScenery::Skills), Menu::ItemSize }, Menu::CharacterSize, m_data.getUIMessage("MenuSkills"_id));
//     m_display.renderString(target, states, { Menu::ItemPosition(GameMenuScenery::Options), Menu::ItemSize }, Menu::CharacterSize, m_data.getUIMessage("MenuOptions"_id));
//     m_display.renderString(target, states, { Menu::ItemPosition(GameMenuScenery::Quit), Menu::ItemSize }, Menu::CharacterSize, m_data.getUIMessage("MenuQuit"_id));
//
//     m_display.renderArrow(target, states, Menu::ArrowPosition(m_scenery.menu.choice));
  }

}