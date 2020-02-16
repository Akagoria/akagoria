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
#include "StartMenuRenderer.h"

#include <gf/Id.h>

#include "ui/Common.h"

using namespace gf::literals;

namespace akgr {

  StartMenuRenderer::StartMenuRenderer(const RootData& data, const OpeningScenery& scenery, ui::Theme& theme)
  : gf::Entity(10)
  , m_data(data)
  , m_scenery(scenery)
  , m_theme(theme)
  , m_frame(nullptr)
  {
    auto menu = m_frame.add<ui::MenuWidget>(m_scenery.menu.index);

    for (gf::Id id : { "MenuStart"_id, "MenuLoad"_id, "MenuOptions"_id, "MenuQuit"_id }) {
      auto label = menu->add<ui::LabelWidget>(m_data.getUIMessage(id));
      label->setSize(ui::Common::DefaultCaptionSize);
    }

    m_frame.setPosition(ui::Common::Position);
    m_frame.computeLayout();
  }

  void StartMenuRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    if (m_scenery.operation != OpeningOperation::Menu) {
      return;
    }

    m_frame.render(target, states, m_theme);
  }

}
