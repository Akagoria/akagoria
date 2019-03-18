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
#ifndef AKGR_COMMON_DISPLAY_H
#define AKGR_COMMON_DISPLAY_H

#include <gf/Alignment.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>
#include <gf/RenderTarget.h>

namespace akgr {

  class Display {
  public:
    Display(gf::ResourceManager& resources);

    void renderArrow(gf::RenderTarget& target, const gf::RenderStates& states, gf::Vector2f position) const;

    void renderBox(gf::RenderTarget& target, const gf::RenderStates& states, const gf::RectF& area) const;
    void renderText(gf::RenderTarget& target, const gf::RenderStates& states, const gf::RectF& area, float characterSize, const std::string& content, gf::Alignment alignment = gf::Alignment::Left) const;
    void renderString(gf::RenderTarget& target, const gf::RenderStates& states, const gf::RectF& area, float characterSize, const std::string& content, gf::Alignment alignment = gf::Alignment::Left) const;
    void renderChoice(gf::RenderTarget& target, const gf::RenderStates& states, const gf::RectF& area, float characterSize, std::string content) const;

    void renderTextBox(gf::RenderTarget& target, const gf::RenderStates& states, const gf::RectF& area, float characterSize, const std::string& content, float padding, gf::Alignment alignment = gf::Alignment::Left) const;

  private:
    gf::Font& m_font;
    const gf::Texture& m_arrow;
  };

}

#endif // AKGR_COMMON_DISPLAY_H
