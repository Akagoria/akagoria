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

#include <gf/VectorOps.h>

namespace akgr {

  namespace {

    constexpr gf::Vector2f SelectorPosition(0.002f, 0.002f);

    constexpr float SelectorCharacterSize = 0.03f;

    constexpr gf::Vector2f SlotPosition(0.03, 0.0f); // first, relative to selector position
    constexpr gf::Vector2f SlotSize(0.17f, 0.11f);
    constexpr float SlotSpacing = 0.007f;

    constexpr gf::Vector2f BackPosition(0.03, 0.0f);
    constexpr gf::Vector2f BackSize(0.22f, 0.04f);

    constexpr gf::Vector2f SelectorSize(SlotPosition.x + SlotSize.x + SlotSpacing, 4 * SlotSize.y + 6 * SlotSpacing + BackSize.y);

    constexpr gf::Vector2f SelectorArrowPosition(0.015f, SlotSize.y / 2); // first, relative to selector position
    constexpr float SelectorArrowGap = SlotSpacing + SlotSize.y;
    constexpr float SelectorArrowCorrection = 0.03f;


    std::string getSlotInfo(const Slot& slot, int index) {
      if (!slot.active) {
        return "(empty)";
      }

      std::string str;

      if (index < SlotSelectorScenery::SlotCount) {
        str += "Slot #" + std::to_string(index);
      } else {
        str += "Quick Save";
      }

      str += '\n';

      str += slot.meta.area + '\n';

      static constexpr std::size_t TimeInfoSize = 1024;

      std::array<char, TimeInfoSize> timeInfo;
      std::strftime(timeInfo.data(), timeInfo.size(), "%F %T", std::localtime(&slot.time));
      str += timeInfo.data();

      return str;
    }

  }

  SlotSelectorRenderer::SlotSelectorRenderer(const SlotSelectorScenery& scenery, const Display& display)
  : m_scenery(scenery)
  , m_display(display)
  {

  }

  SlotSelectorRenderer::~SlotSelectorRenderer() = default;

  void SlotSelectorRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    if (!isDisplayed()) {
      return;
    }

    m_display.renderBox(target, states, { SelectorPosition, SelectorSize });

    gf::Vector2f position = SelectorPosition + SlotPosition;
    position.y += SlotSpacing;

    m_display.renderBox(target, states, { position, SlotSize });
    m_display.renderText(target, states, { position + SlotSpacing, SlotSize }, 0.8f * SelectorCharacterSize, getSlotInfo(m_scenery.manual[0], 0));

    position.y += SlotSize.height + SlotSpacing;

    m_display.renderBox(target, states, { position, SlotSize });
    m_display.renderText(target, states, { position + SlotSpacing, SlotSize }, 0.8f * SelectorCharacterSize, getSlotInfo(m_scenery.manual[1], 1));

    position.y += SlotSize.height + SlotSpacing;

    m_display.renderBox(target, states, { position, SlotSize });
    m_display.renderText(target, states, { position + SlotSpacing, SlotSize }, 0.8f * SelectorCharacterSize, getSlotInfo(m_scenery.manual[2], 2));

    position.y += SlotSize.height + SlotSpacing;

    m_display.renderBox(target, states, { position, SlotSize });
    m_display.renderText(target, states, { position + SlotSpacing, SlotSize }, 0.8f * SelectorCharacterSize, getSlotInfo(m_scenery.quick, 3));

    position.y += SlotSize.height + 2 * SlotSpacing;

    m_display.renderText(target, states, { position, SlotSize }, SelectorCharacterSize, "Back");

    gf::Vector2f arrowPosition = SelectorPosition + SelectorArrowPosition;

    if (m_scenery.choice < SlotSelectorScenery::SlotCount + 1) {
      arrowPosition.y += SelectorArrowGap * m_scenery.choice;
    } else {
      arrowPosition.y += SelectorArrowGap * (SlotSelectorScenery::SlotCount + 1) - SelectorArrowCorrection;
    }

    m_display.renderArrow(target, states, arrowPosition);
  }


  OpeningSlotSelectorRenderer::OpeningSlotSelectorRenderer(const OpeningScenery& scenery, const Display& display)
  : SlotSelectorRenderer(scenery.selector, display)
  , m_scenery(scenery)
  {

  }

  bool OpeningSlotSelectorRenderer::isDisplayed() const {
    return m_scenery.operation == OpeningOperation::Select;
  }

  WorldSlotSelectorRenderer::WorldSlotSelectorRenderer(const WorldState& state, const WorldScenery& scenery, const Display& display)
  : SlotSelectorRenderer(scenery.selector, display)
  , m_state(state)
  {

  }

  bool WorldSlotSelectorRenderer::isDisplayed() const {
    return m_state.operation == WorldOperation::Save;
  }

}
