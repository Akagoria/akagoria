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
#include "WorldDialogScene.h"

#include <gf/Unused.h>

#include "Akagoria.h"
#include "WorldAct.h"

namespace akgr {

  WorldDialogScene::WorldDialogScene(Akagoria& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_dialog(game.world.state, game.theme)
  {
    addHudEntity(m_dialog);

    addAction(m_game.commands.gameUse);
  }

  void WorldDialogScene::doHandleActions(gf::Window& window) {
    gf::unused(window);

    auto& hero = m_game.world.state.hero;

    m_game.root.scenery.helper.status = HelperStatus::Continue;

    hero.move.angular = gf::AngularMove::None;
    hero.move.linear = gf::LinearMove::None;

    if (m_game.commands.gameUse.isActive()) {
      auto& dialog = hero.dialog;
      assert(dialog.ref.data != nullptr);

      ++dialog.currentLine;

      if (dialog.currentLine >= dialog.ref.data->content.size()) {
        // end of the dialog
        std::string name = dialog.ref.data->name;
        dialog.ref.data = nullptr;
        dialog.ref.id = gf::InvalidId;
        dialog.currentLine = 0;
        m_game.world.script.onDialog(name);

        m_game.replaceScene(m_game.worldAct->travel);
      }
    }

  }

}
