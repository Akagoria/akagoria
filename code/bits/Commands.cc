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

  Commands::Commands()
  : windowClose("Close window")
  , windowFullscreen("Fullscreen")
  , debugPhysics("Debug Physics")
  , debugSave("Debug Save")
  , gameUp("Up")
  , gameDown("Down")
  , gameLeft("Left")
  , gameRight("Right")
  , gameMove("Move")
  , gameUse("Use")
  , gameFight("Fight")
  , gameMenu("Menu")
  , menuUp("Menu Up")
  , menuDown("Menu Down")
  , menuLeft("Menu Left")
  , menuRight("Menu Right")
  , menuPageUp("Menu Page Up")
  , menuPageDown("Menu Page Down")
  , menuQuit("Menu Quit")
  {
    windowClose.addKeycodeKeyControl(gf::Keycode::Escape);

    windowFullscreen.addKeycodeKeyControl(gf::Keycode::F);

    debugPhysics.addScancodeKeyControl(gf::Scancode::F10);

    debugSave.addScancodeKeyControl(gf::Scancode::F11);

    gameUp.addScancodeKeyControl(gf::Scancode::W);
    gameUp.addScancodeKeyControl(gf::Scancode::Up);
//     gameUp.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Negative);
    gameUp.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadUp);
    gameUp.setContinuous();

    gameDown.addScancodeKeyControl(gf::Scancode::S);
    gameDown.addScancodeKeyControl(gf::Scancode::Down);
//     gameDown.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Positive);
    gameDown.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadDown);
    gameDown.setContinuous();

    gameLeft.addScancodeKeyControl(gf::Scancode::A);
    gameLeft.addScancodeKeyControl(gf::Scancode::Left);
//     gameLeft.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftX, gf::GamepadAxisDirection::Negative);
    gameLeft.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadLeft);
    gameLeft.setContinuous();

    gameRight.addScancodeKeyControl(gf::Scancode::D);
    gameRight.addScancodeKeyControl(gf::Scancode::Right);
//     gameRight.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftX, gf::GamepadAxisDirection::Positive);
    gameRight.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadRight);
    gameRight.setContinuous();

    gameMove.addControl(gamepad);
    gameMove.setContinuous();

    gameUse.addKeycodeKeyControl(gf::Keycode::E);
    gameUse.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::A);

    gameFight.addKeycodeKeyControl(gf::Keycode::R);
    gameFight.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::B);

    gameMenu.addKeycodeKeyControl(gf::Keycode::M);
    gameMenu.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Y);

    menuUp.addScancodeKeyControl(gf::Scancode::W);
    menuUp.addScancodeKeyControl(gf::Scancode::Up);
    menuUp.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Negative);
    menuUp.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadUp);

    menuDown.addScancodeKeyControl(gf::Scancode::S);
    menuDown.addScancodeKeyControl(gf::Scancode::Down);
    menuDown.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Positive);
    menuDown.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadDown);

    menuLeft.addScancodeKeyControl(gf::Scancode::A);
    menuLeft.addScancodeKeyControl(gf::Scancode::Left);
    menuLeft.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftX, gf::GamepadAxisDirection::Negative);
    menuLeft.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadLeft);

    menuRight.addScancodeKeyControl(gf::Scancode::D);
    menuRight.addScancodeKeyControl(gf::Scancode::Right);
    menuRight.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftX, gf::GamepadAxisDirection::Positive);
    menuRight.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadRight);

    menuPageUp.addScancodeKeyControl(gf::Scancode::PageUp);
    menuPageUp.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::TriggerRight, gf::GamepadAxisDirection::Positive);
    menuPageUp.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::RightBumper);

    menuPageDown.addScancodeKeyControl(gf::Scancode::PageDown);
    menuPageDown.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::TriggerLeft, gf::GamepadAxisDirection::Positive);
    menuPageDown.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::LeftBumper);

    menuQuit.addKeycodeKeyControl(gf::Keycode::R);
    menuQuit.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::B);
  }

}
