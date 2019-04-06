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
#ifndef AKGR_COMMANDS_H
#define AKGR_COMMANDS_H

#include <gf/Action.h>

namespace akgr {

  struct Commands {
    gf::Action up;
    gf::Action down;
    gf::Action left;
    gf::Action right;

    gf::Action use;
    gf::Action fight;
    gf::Action menu;

    gf::Action menuUp;
    gf::Action menuDown;
    gf::Action menuLeft;
    gf::Action menuRight;
    gf::Action menuPageUp;
    gf::Action menuPageDown;
    gf::Action menuQuit;

    Commands(gf::ActionContainer& container);
  };

}

#endif // AKGR_ACTIONS_H
