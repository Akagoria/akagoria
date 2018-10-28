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
#include "Commands.h"

namespace akgr {

  Commands::Commands(gf::ActionContainer& container)
  : up("Up")
  , down("Down")
  , left("Left")
  , right("Right")
  , use("Use")
  , fight("Fight")
  , menuUp("Menu Up")
  , menuDown("Menu Down")
  {
    up.addScancodeKeyControl(gf::Scancode::W);
    up.addScancodeKeyControl(gf::Scancode::Up);
    up.setContinuous();
    container.addAction(up);

    down.addScancodeKeyControl(gf::Scancode::S);
    down.addScancodeKeyControl(gf::Scancode::Down);
    down.setContinuous();
    container.addAction(down);

    left.addScancodeKeyControl(gf::Scancode::A);
    left.addScancodeKeyControl(gf::Scancode::Left);
    left.setContinuous();
    container.addAction(left);

    right.addScancodeKeyControl(gf::Scancode::D);
    right.addScancodeKeyControl(gf::Scancode::Right);
    right.setContinuous();
    container.addAction(right);

    use.addKeycodeKeyControl(gf::Keycode::E);
    container.addAction(use);

    fight.addKeycodeKeyControl(gf::Keycode::Space);
    container.addAction(fight);

    menuUp.addScancodeKeyControl(gf::Scancode::W);
    menuUp.addScancodeKeyControl(gf::Scancode::Up);
    container.addAction(menuUp);

    menuDown.addScancodeKeyControl(gf::Scancode::S);
    menuDown.addScancodeKeyControl(gf::Scancode::Down);
    container.addAction(menuDown);

  }

}
