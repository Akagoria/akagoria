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
#include "OpeningMenuScene.h"

#include "GameScenes.h"
#include "OpeningAct.h"

namespace akgr {

  OpeningMenuScene::OpeningMenuScene(GameScenes& scenes)
  : gf::Scene(scenes.getRenderer().getSize())
  , m_scenes(scenes)
  , m_menu(scenes.root.data, scenes.opening.scenery, scenes.theme)
  {
    addHudEntity(m_menu);

    addAction(m_scenes.commands.menuUp);
    addAction(m_scenes.commands.menuDown);
    addAction(m_scenes.commands.gameUse);
  }

  void OpeningMenuScene::doHandleActions(gf::Window& window) {
    if (m_scenes.commands.menuDown.isActive()) {
      m_scenes.opening.scenery.menu.index.computeNextChoice();
    } else if (m_scenes.commands.menuUp.isActive()) {
      m_scenes.opening.scenery.menu.index.computePrevChoice();
    }

    if (m_scenes.commands.gameUse.isActive()) {
      switch (m_scenes.opening.scenery.menu.getChoice()) {
        case StartMenuScenery::Choice::StartAdventure:
//           m_scenery.operation = OpeningOperation::Start;
          break;

        case StartMenuScenery::Choice::LoadAdventure:
//           m_scenery.operation = OpeningOperation::SelectSlot;
          break;

        case StartMenuScenery::Choice::Options:
          m_scenes.replaceScene(m_scenes.openingAct->options);
//           m_scenery.operation = OpeningOperation::ChangeOptions;
          break;

        case StartMenuScenery::Choice::Quit:
//           m_scenery.operation = OpeningOperation::Quit;
          m_scenes.popAllScenes();
          break;
      }
    }
  }

}
