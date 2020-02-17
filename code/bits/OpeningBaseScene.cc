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
#include "OpeningBaseScene.h"

#include <gf/Log.h>

#include "GameScenes.h"
#include "Opening.h"
#include "Root.h"

namespace akgr {

  OpeningBaseScene::OpeningBaseScene(GameScenes& scenes)
  : gf::Scene(scenes.getRenderer().getSize())
  , m_scenes(scenes)
  , m_logo(scenes.root.data, scenes.opening.scenery, scenes.resources)
  , m_helper(scenes.root.data, scenes.root.scenery, scenes.resources)
  {
    addHudEntity(m_logo);
    addHudEntity(m_helper);

    addAction(m_scenes.commands.windowClose);
    addAction(m_scenes.commands.windowFullscreen);
  }

  void OpeningBaseScene::doHandleActions(gf::Window& window) {
    if (m_scenes.commands.windowFullscreen.isActive()) {
      window.toggleFullscreen();
    }

    if (m_scenes.commands.windowClose.isActive()) {
      window.close();
    }
  }

}
