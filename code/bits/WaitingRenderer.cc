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
#include "WaitingRenderer.h"

#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/Id.h>
#include <gf/RenderTarget.h>
#include <gf/Text.h>

using namespace gf::literals;

namespace akgr {

  WaitingRenderer::WaitingRenderer(const RootData& data, gf::ResourceManager& resources)
  : m_data(data)
  , m_mainFont(resources.getFont("fonts/Philosopher-Regular.ttf"))
  {
  }

  void WaitingRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coords(target);

    unsigned loadingCharacterSize = coords.getRelativeCharacterSize(0.05f);
    gf::Vector2f loadingPosition = coords.getRelativePoint({ 0.5f, 0.85f });

    gf::Text loadingText(m_data.getUIMessage("SplashLoading"_id), m_mainFont, loadingCharacterSize);
    loadingText.setLetterSpacing(1.5f);
    loadingText.setPosition(loadingPosition);
    loadingText.setAnchor(gf::Anchor::Center);
    target.draw(loadingText, states);
  }

}
