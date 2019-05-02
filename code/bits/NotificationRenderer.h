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
#ifndef AKGR_NOTIFICATION_RENDERER_H
#define AKGR_NOTIFICATION_RENDERER_H

#include <gf/Entity.h>

#include "WorldState.h"

#include "ui/Theme.h"

namespace akgr {

  class NotificationRenderer : public gf::Entity {
  public:
    NotificationRenderer(const WorldState& state, ui::Theme& theme);

    virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    const WorldState& m_state;
    ui::Theme& m_theme;
  };

}


#endif // AKGR_NOTIFICATION_RENDERER_H
