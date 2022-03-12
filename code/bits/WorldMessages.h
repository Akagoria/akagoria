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
#ifndef AKGR_WORLD_MESSAGES_H
#define AKGR_WORLD_MESSAGES_H

#include <gf/Message.h>
#include <gf/Time.h>

#include "Aspect.h"
#include "LandscapeData.h"
#include "Value.h"

namespace akgr {

  using namespace gf::literals;

  enum class DamageReceiver {
    Hero,
    Other,
  };

  struct DamageGenerationMessage : gf::Message {
    static constexpr gf::Id type = "DamageGenerationMessage"_id;

    DamageReceiver receiver;
    Value value;
    gf::Vector2f position;
  };


  struct AspectChangeMessage : gf::Message {
    static constexpr gf::Id type = "AspectChangeMessage"_id;

    LandscapeShrineData *shrine;
    Aspect aspect;
    gf::Time time;
  };
}

#endif // AKGR_WORLD_MESSAGES_H
