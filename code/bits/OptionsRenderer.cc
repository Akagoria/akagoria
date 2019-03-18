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
#include "OptionsRenderer.h"

#include <boost/locale.hpp>

#include <gf/Id.h>
#include <gf/VectorOps.h>

#include "Menu.h"

using namespace gf::literals;

namespace akgr {

  namespace {
    constexpr gf::Vector2f OptionSize = { Menu::ItemSize.x * 2, Menu::ItemSize.y };

    constexpr gf::Vector2f OptionsTotalSize(int optionCount) {
      return gf::Vector2f(Menu::Position.x + Menu::ItemRelPosition.x + OptionSize.x + Menu::ItemSpacing, optionCount * OptionSize.y + (optionCount + 1) * Menu::ItemSpacing);
    }

    constexpr gf::Vector2f ChoicePosition(int choice) {
      gf::Vector2f position = Menu::ItemPosition(choice);
      position.x += Menu::ItemSize.x;
      return position;
    }

    std::string getInputChoice(Options::Input choice, const UIData& data) {
      switch (choice) {
        case Options::Input::Keyboard_Arrows:
          return data.getUIMessage("MenuOptionsInputKeyboardKeys"_id);
        case Options::Input::Keyboard_Keys:
          return data.getUIMessage("MenuOptionsInputKeyboardArrows"_id);
        case Options::Input::Gamepad:
          return data.getUIMessage("MenuOptionsInputGamepad"_id);
      }

      return "???";
    }

    std::string getDisplayChoice(Options::Display choice, const UIData& data) {
      switch (choice) {
        case Options::Display::Fullscreen:
          return data.getUIMessage("MenuOptionsDisplayFullscreen"_id);
        case Options::Display::Window_960x540:
          return data.getUIMessage("MenuOptionsDisplayWindow960x540"_id);
        case Options::Display::Window_1024x576:
          return data.getUIMessage("MenuOptionsDisplayWindow1024x576"_id);
        case Options::Display::Window_1152x648:
          return data.getUIMessage("MenuOptionsDisplayWindow1152x648"_id);
        case Options::Display::Window_1280x720:
          return data.getUIMessage("MenuOptionsDisplayWindow1280x720"_id);
      }

      return "???";
    }

  }

  OptionsRenderer::OptionsRenderer(const UIData& data, const RootScenery& scenery, const Display& display)
  : m_data(data)
  , m_scenery(scenery)
  , m_display(display)
  {

  }

  void OptionsRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    if (m_scenery.operation != RootOperation::ChangeOptions) {
      return;
    }

    m_display.renderBox(target, states, { Menu::Position, OptionsTotalSize(3) });
    m_display.renderString(target, states, { Menu::ItemPosition(0), Menu::ItemSize }, Menu::CharacterSize, m_data.getUIMessage("MenuOptionsInput"_id));
    m_display.renderChoice(target, states, { ChoicePosition(0), Menu::ItemSize }, Menu::CharacterSize, getInputChoice(m_scenery.options.options.input, m_data));
    m_display.renderString(target, states, { Menu::ItemPosition(1), Menu::ItemSize }, Menu::CharacterSize, m_data.getUIMessage("MenuOptionsDisplay"_id));
    m_display.renderChoice(target, states, { ChoicePosition(1), Menu::ItemSize }, Menu::CharacterSize, getDisplayChoice(m_scenery.options.options.display, m_data));
    m_display.renderString(target, states, { Menu::ItemPosition(2), Menu::ItemSize }, Menu::CharacterSize, m_data.getUIMessage("MenuBack"_id));

    m_display.renderArrow(target, states, Menu::ArrowPosition(m_scenery.options.choice));
  }

}
