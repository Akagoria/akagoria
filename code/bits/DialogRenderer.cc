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
#include "DialogRenderer.h"

#include <gf/Log.h>

namespace akgr {

  namespace {

    constexpr gf::Vector2f SpeakerPosition(0.19f, 0.75f);
    constexpr gf::Vector2f SpeakerSize(0.2f, 0.03f);
    constexpr float SpeakerCharacterSize = 0.03;

    constexpr gf::Vector2f WordsPosition(0.17f, 0.8f);
    constexpr gf::Vector2f WordsSize(2.0f * (0.5f - WordsPosition.x), 0.11f);
    constexpr float WordsCharacterSize = 0.035;

  } // anonymous namespace

  DialogRenderer::DialogRenderer(const WorldState& state, const Display& display)
  : gf::Entity(50)
  , m_state(state)
  , m_display(display)
  {

  }

  void DialogRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    auto& dialog = m_state.hero.dialog;

    if (dialog.ref.data == nullptr) {
      assert(dialog.ref.id == gf::InvalidId);
      return;
    }

    assert(dialog.currentLine < dialog.ref.data->content.size());
    auto& line = dialog.ref.data->content[dialog.currentLine];

    m_display.renderTextBox(target, states, { SpeakerPosition, SpeakerSize }, SpeakerCharacterSize, line.speaker, gf::Alignment::Left);
    m_display.renderTextBox(target, states, { WordsPosition, WordsSize }, WordsCharacterSize, line.words, gf::Alignment::Left);
  }

}

