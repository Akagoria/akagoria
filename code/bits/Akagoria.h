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
#ifndef AKGR_AKAGORIA_H
#define AKGR_AKAGORIA_H

#include <memory>

#include <gf/Gamepad.h>
#include <gf/Random.h>
#include <gf/ResourceManager.h>
#include <gf/SceneManager.h>

#include "Commands.h"
#include "Opening.h"
#include "Root.h"
#include "World.h"

#include "ui/Theme.h"

namespace akgr {

  struct OpeningAct;
  struct WorldAct;

  struct Akagoria : public gf::SceneManager {
    Akagoria(gf::Path searchDirectory);
    ~Akagoria();

    Akagoria(const Akagoria&) = delete;
    Akagoria(Akagoria&&) = delete;

    Akagoria& operator=(const Akagoria&) = delete;
    Akagoria& operator=(Akagoria&&) = delete;

    void startOpening();

    gf::ResourceManager resources;
    gf::Random random;
    gf::GamepadTracker tracker;

    ui::Theme theme;

    Commands commands;

    Root root;
    Opening opening;
    World world;

    std::unique_ptr<OpeningAct> openingAct;
    std::unique_ptr<WorldAct> worldAct;
  };

}

#endif // AKGR_AKAGORIA_H
