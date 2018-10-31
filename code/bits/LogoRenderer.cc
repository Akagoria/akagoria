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
#include "LogoRenderer.h"

#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/VectorOps.h>

namespace akgr {

  namespace {

    constexpr float LogoHeight = 347.0f;

    constexpr float RelativeLineHeight = 0.005;
    constexpr float RelativePaddingAroundLine = 0.01f + RelativeLineHeight / 2;

  }

  LogoRenderer::LogoRenderer(const OpeningScenery& scenery, gf::ResourceManager& resources)
  : m_scenery(scenery)
  , m_texture(resources.getTexture("logo.png"))
  , m_mainFont(resources.getFont("fonts/Philosopher-Regular.ttf"))
  , m_additionalFont(resources.getFont("fonts/sawarabi-mincho-medium.ttf"))
  , m_subtitleFont(resources.getFont("fonts/Philosopher-Italic.ttf"))
  {
    resources.getTexture("logo.png").setSmooth();
  }

  void LogoRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::Coordinates coords(target);

    float height = coords.getRelativeSize({ 0.0f, 0.5f }).height;

    gf::Sprite logo(m_texture);
    logo.setColor(gf::Color::Gray(0.8f));
    logo.scale(height / LogoHeight);
    logo.setPosition(coords.getCenter());
    logo.setAnchor(gf::Anchor::Center);
    target.draw(logo, states);

    unsigned mainCharacterSize = coords.getRelativeCharacterSize(0.15f);
    gf::Vector2f mainPosition = coords.getRelativePoint({ 0.5f, 0.5f - RelativePaddingAroundLine });

    gf::Text mainText("AKAGORIA", m_mainFont, mainCharacterSize);
    mainText.setLetterSpacing(2.0f);
    mainText.setPosition(mainPosition);
    mainText.setAnchor(gf::Anchor::BottomCenter);
    target.draw(mainText, states);

    unsigned additionalcharacterSize = coords.getRelativeCharacterSize(0.075f);
    gf::Vector2f additionalPosition = mainText.getPosition() + mainText.getLocalBounds().getBottomRight() / 2 + coords.getRelativeSize({0.0f, 2 * RelativePaddingAroundLine });

    gf::Text additionalText("アカゴリア", m_additionalFont, additionalcharacterSize);
    additionalText.setPosition(additionalPosition);
    additionalText.setAnchor(gf::Anchor::TopRight);
    target.draw(additionalText, states);


    gf::Vector2f lineSize = coords.getRelativeSize({ 0.55f, RelativeLineHeight });

    gf::RectangleShape line(lineSize);
    line.setPosition(coords.getCenter());
    line.setAnchor(gf::Anchor::Center);
    line.setColor(gf::Color::Black);
    target.draw(line, states);

    unsigned subtitlecharacterSize = coords.getRelativeCharacterSize(0.06f);
    gf::Vector2f subtitlePosition = coords.getRelativePoint({ 0.5f, 0.65f });

    gf::Text subtitleText("The revenge of Kalista", m_subtitleFont, subtitlecharacterSize);
//     subtitleText.setLetterSpacing(1.5f);
    subtitleText.setPosition(subtitlePosition);
    subtitleText.setAnchor(gf::Anchor::Center);
    target.draw(subtitleText, states);

    if (m_scenery.operation == OpeningOperation::Wait) {
      unsigned loadingCharacterSize = coords.getRelativeCharacterSize(0.05f);
      gf::Vector2f loadingPosition = coords.getRelativePoint({ 0.5f, 0.85f });

      gf::Text loadingText("Loading...", m_mainFont, loadingCharacterSize);
      loadingText.setLetterSpacing(1.5f);
      loadingText.setPosition(loadingPosition);
      loadingText.setAnchor(gf::Anchor::Center);
      target.draw(loadingText, states);
    }

  }

}
