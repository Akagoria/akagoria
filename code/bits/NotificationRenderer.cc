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
#include "NotificationRenderer.h"

#include <boost/locale.hpp>

#include "ui/Common.h"
#include "ui/Widgets.h"

namespace akgr {

  namespace {

    constexpr gf::Vector2f NotificationSize(0.4f, 1.3f * ui::Common::DefaultCharacterSize);
    constexpr gf::Vector2f NotificationPosition((1.0f - NotificationSize.x) / 2.0f, 0.1f);

  } // anonymous namespace

  NotificationRenderer::NotificationRenderer(const WorldState& state, ui::Theme& theme)
  : gf::Entity(50)
  , m_state(state)
  , m_theme(theme)
  {

  }

  void NotificationRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    if (m_state.notifications.empty()) {
      return;
    }

    auto& notification = m_state.notifications.front();

    ui::FrameWidget frame(nullptr);
    auto text = frame.add<ui::TextWidget>(boost::locale::gettext(notification.ref().message.c_str()), ui::Common::DefaultCharacterSize, gf::Alignment::Center);
    text->setSize(NotificationSize);
    frame.setPosition(NotificationPosition);
    frame.setMargin({ 0.01f, 0.01f });
    frame.computeLayout();
    frame.render(target, states, m_theme);
  }

}

