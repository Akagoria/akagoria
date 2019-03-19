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
#include "SlotSelectorRenderer.h"

#include <ctime>
#include <array>
#include <string>

#include <boost/locale.hpp>

#include <gf/Id.h>
#include <gf/VectorOps.h>

#include "Menu.h"

using namespace gf::literals;

namespace akgr {

  namespace {

    constexpr gf::Vector2f SlotRelPosition(0.03 + Menu::ItemSpacing, Menu::ItemSpacing); // first, relative to selector position
    constexpr gf::Vector2f SlotSize(Menu::ItemSize.x, Menu::CharacterSize * 1.2 * 3.0f);
    constexpr gf::Vector2f ExtendedSlotSize = SlotSize + 2 * Menu::ItemSpacing;

    constexpr gf::Vector2f SelectorTotalSize(int slotCount) {
      return gf::Vector2f(SlotRelPosition.x + ExtendedSlotSize.x, slotCount * ExtendedSlotSize.y + (slotCount + 2) * Menu::ItemSpacing + Menu::ItemSize.y);
    }

    static constexpr gf::Vector2f SlotPosition(int item) {
      gf::Vector2f pos = Menu::Position + SlotRelPosition;
      pos.y += Menu::ItemSpacing + item * (Menu::ItemSpacing + ExtendedSlotSize.y);
      return pos;
    }

    constexpr gf::Vector2f SelectorArrowPosition(0.015f, ExtendedSlotSize.y / 2); // first, relative to selector position
    constexpr float SelectorArrowGap = Menu::ItemSpacing + ExtendedSlotSize.y;
    constexpr float SelectorArrowCorrection = 0.03f;


    std::string getSlotInfo(const Slot& slot, int index) {
      if (!slot.active) {
        return "Slot #" + std::to_string(index) + "\n---\n---";
      }

      std::string str;

      if (index < SlotSelectorScenery::SlotCount) {
        str += "Slot #" + std::to_string(index);
      } else {
        str += "Quick Save";
      }

      str += '\n';

      str += boost::locale::gettext(slot.meta.area.c_str()) + '\n';

      static constexpr std::size_t TimeInfoSize = 1024;

      std::array<char, TimeInfoSize> timeInfo;
      std::strftime(timeInfo.data(), timeInfo.size(), "%F %T", std::localtime(&slot.time));
      str += timeInfo.data();

      return str;
    }

  }

  SlotSelectorRenderer::SlotSelectorRenderer(const UIData& data, const RootScenery& scenery, const Display& display)
  : m_data(data)
  , m_scenery(scenery)
  , m_display(display)
  {

  }

  void SlotSelectorRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    if (m_scenery.operation != RootOperation::SelectSlot) {
      return;
    }

    m_display.renderBox(target, states, { Menu::Position, SelectorTotalSize(SlotSelectorScenery::SlotCount + 1) });

    m_display.renderTextBox(target, states, { SlotPosition(0), SlotSize }, Menu::CharacterSize, getSlotInfo(m_scenery.selector.manual[0], 0), Menu::ItemSpacing);
    m_display.renderTextBox(target, states, { SlotPosition(1), SlotSize }, Menu::CharacterSize, getSlotInfo(m_scenery.selector.manual[1], 1), Menu::ItemSpacing);
    m_display.renderTextBox(target, states, { SlotPosition(2), SlotSize }, Menu::CharacterSize, getSlotInfo(m_scenery.selector.manual[2], 2), Menu::ItemSpacing);
    m_display.renderTextBox(target, states, { SlotPosition(3), SlotSize }, Menu::CharacterSize, getSlotInfo(m_scenery.selector.quick, 3), Menu::ItemSpacing);

    m_display.renderString(target, states, { SlotPosition(4) - Menu::ItemSpacing, Menu::ItemSize }, Menu::CharacterSize, m_data.getUIMessage("MenuBack"_id));

    // arrow

    gf::Vector2f arrowPosition = Menu::Position + SelectorArrowPosition;

    if (m_scenery.selector.choice < SlotSelectorScenery::SlotCount + 1) {
      arrowPosition.y += Menu::ItemSpacing + SelectorArrowGap * m_scenery.selector.choice;
    } else {
      arrowPosition.y += Menu::ItemSpacing + SelectorArrowGap * (SlotSelectorScenery::SlotCount + 1) - ExtendedSlotSize.y / 2 + Menu::ItemSize.y / 2;
    }

    m_display.renderArrow(target, states, arrowPosition);
  }

}
