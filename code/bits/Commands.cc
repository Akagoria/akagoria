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
  , menu("Menu")
  , menuUp("Menu Up")
  , menuDown("Menu Down")
  , menuLeft("Menu Left")
  , menuRight("Menu Right")
  {
    up.addScancodeKeyControl(gf::Scancode::W);
    up.addScancodeKeyControl(gf::Scancode::Up);
    up.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Negative);
    up.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadUp);
    up.setContinuous();
    container.addAction(up);

    down.addScancodeKeyControl(gf::Scancode::S);
    down.addScancodeKeyControl(gf::Scancode::Down);
    down.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Positive);
    down.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadDown);
    down.setContinuous();
    container.addAction(down);

    left.addScancodeKeyControl(gf::Scancode::A);
    left.addScancodeKeyControl(gf::Scancode::Left);
    left.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftX, gf::GamepadAxisDirection::Negative);
    left.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadLeft);
    left.setContinuous();
    container.addAction(left);

    right.addScancodeKeyControl(gf::Scancode::D);
    right.addScancodeKeyControl(gf::Scancode::Right);
    right.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftX, gf::GamepadAxisDirection::Positive);
    right.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadRight);
    right.setContinuous();
    container.addAction(right);

    use.addKeycodeKeyControl(gf::Keycode::E);
    use.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::A);
    container.addAction(use);

    fight.addKeycodeKeyControl(gf::Keycode::R);
    fight.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::B);
    container.addAction(fight);

    menu.addKeycodeKeyControl(gf::Keycode::M);
    menu.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Y);
    container.addAction(menu);

    menuUp.addScancodeKeyControl(gf::Scancode::W);
    menuUp.addScancodeKeyControl(gf::Scancode::Up);
    menuUp.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Negative);
    menuUp.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadUp);
    container.addAction(menuUp);

    menuDown.addScancodeKeyControl(gf::Scancode::S);
    menuDown.addScancodeKeyControl(gf::Scancode::Down);
    menuDown.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Positive);
    menuDown.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadDown);
    container.addAction(menuDown);

    menuLeft.addScancodeKeyControl(gf::Scancode::A);
    menuLeft.addScancodeKeyControl(gf::Scancode::Left);
    menuLeft.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftX, gf::GamepadAxisDirection::Negative);
    menuLeft.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadLeft);
    container.addAction(menuLeft);

    menuRight.addScancodeKeyControl(gf::Scancode::D);
    menuRight.addScancodeKeyControl(gf::Scancode::Right);
    menuRight.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftX, gf::GamepadAxisDirection::Positive);
    menuRight.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadRight);
    container.addAction(menuRight);
  }

}
