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

#include <gf/ArrayRef.h>
#include <gf/Id.h>

#include "ui/Common.h"

using namespace gf::literals;

namespace akgr {

  OptionsRenderer::OptionsRenderer(const UIData& data, const RootScenery& scenery, ui::Theme& theme)
  : m_data(data)
  , m_scenery(scenery)
  , m_theme(theme)
  , m_frame(nullptr)
  {

    auto addOption = [this](ui::ContainerWidget* parent, const ui::WidgetIndexScenery& index, gf::Id caption, gf::ArrayRef<gf::Id> choices) {
      auto line = parent->add<ui::BoxWidget>(ui::Aspect::Horizontal);
      line->add<ui::LabelWidget>(m_data.getUIMessage(caption))->setSize(ui::Common::DefaultCaptionSize);
      auto choice = line->add<ui::ChoiceWidget>(index);

      for (gf::Id id : choices) {
        auto text = choice->add<ui::TextWidget>(m_data.getUIMessage(id), ui::Common::DefaultCharacterSize, gf::Alignment::Center);
        text->setSize(ui::Common::DefaultCaptionSize);
      }
    };

    auto menu = m_frame.add<ui::MenuWidget>(m_scenery.options.index);

    addOption(menu, m_scenery.options.data.input, "MenuOptionsInput"_id,
        { "MenuOptionsInputKeyboard"_id, "MenuOptionsInputGamepad"_id }
    );

    addOption(menu, m_scenery.options.data.display, "MenuOptionsDisplay"_id,
        { "MenuOptionsDisplayFullscreen"_id,  "MenuOptionsDisplayWindow960x540"_id, "MenuOptionsDisplayWindow1024x576"_id, "MenuOptionsDisplayWindow1152x648"_id, "MenuOptionsDisplayWindow1280x720"_id }
    );

    auto label = menu->add<ui::LabelWidget>(m_data.getUIMessage("MenuBack"_id));
    label->setSize(ui::Common::DefaultCaptionSize);

    m_frame.setPosition(ui::Common::Position);
    m_frame.computeLayout();
  }

  void OptionsRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    if (m_scenery.operation != RootOperation::ChangeOptions) {
      return;
    }

    m_frame.render(target, states, m_theme);
  }

}
