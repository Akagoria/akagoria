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
#include "GameMenuRenderer.h"

#include <gf/VectorOps.h>

#include "ui/Common.h"

using namespace gf::literals;

namespace akgr {

  GameMenuRenderer::GameMenuRenderer(const UIData& data, const WorldState& state, const WorldScenery& scenery, ui::Theme& theme)
  : gf::Entity(10)
  , m_data(data)
  , m_state(state)
  , m_scenery(scenery)
  , m_theme(theme)
  , m_frame(nullptr)
  {
    auto menu = m_frame.add<ui::MenuWidget>(m_scenery.menu.index);

    for (gf::Id id : { "MenuInventory"_id, "MenuQuests"_id, "MenuSkills"_id, "MenuOptions"_id, "MenuBackToAdventure"_id, "MenuBackToRealLife"_id }) {
      auto label = menu->add<ui::LabelWidget>(m_data.getUIMessage(id));
      label->setSize(ui::Common::DefaultCaptionSize);
    }

    m_frame.setPosition(ui::Common::Position);
    m_frame.computeLayout();
  }

  void GameMenuRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    if (m_state.operation != WorldOperation::Menu) {
      return;
    }

    m_frame.render(target, states, m_theme);
  }

}
