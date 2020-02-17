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
#include "OpeningOptionsScene.h"

#include <gf/Log.h>

#include "GameScenes.h"
#include "OpeningAct.h"

namespace akgr {

  OpeningOptionsScene::OpeningOptionsScene(GameScenes& scenes)
  : gf::Scene(scenes.getRenderer().getSize())
  , m_scenes(scenes)
  , m_options(scenes.root.data, scenes.root.scenery, scenes.theme)
  {
    addHudEntity(m_options);

    addAction(m_scenes.commands.menuUp);
    addAction(m_scenes.commands.menuDown);
    addAction(m_scenes.commands.menuLeft);
    addAction(m_scenes.commands.menuRight);
    addAction(m_scenes.commands.gameUse);
  }

  void OpeningOptionsScene::doHandleActions(gf::Window& window) {
    if (m_scenes.commands.menuDown.isActive()) {
      m_scenes.root.scenery.options.index.computeNextChoice();
    } else if (m_scenes.commands.menuUp.isActive()) {
      m_scenes.root.scenery.options.index.computePrevChoice();
    }

    if (m_scenes.commands.gameUse.isActive()) {
      if (m_scenes.root.scenery.options.getChoice() == OptionsScenery::Choice::Back) {
//         m_scenery.operation = OpeningOperation::Menu;
//         m_root.operation = RootOperation::None;
        m_scenes.root.scenery.options.save();
        m_scenes.replaceScene(m_scenes.openingAct->menu);
      }
    }

    if (m_scenes.commands.menuRight.isActive()) {
      m_scenes.root.scenery.options.computeNextOption();
    }

    if (m_scenes.commands.menuLeft.isActive()) {
      m_scenes.root.scenery.options.computePrevOption();
    }
  }

}
