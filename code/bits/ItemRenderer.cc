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
#include "ItemRenderer.h"

#include <gf/Color.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>

namespace akgr {

  ItemRenderer::ItemRenderer(const WorldData& data, const WorldState& state, gf::ResourceManager& resources)
  : m_data(data)
  , m_state(state)
  , m_resources(resources)
  {
  }

  void ItemRenderer::renderFloor(gf::RenderTarget& target, const gf::RenderStates& states, int32_t floor) {
    for (auto& item : m_state.items) {
      if (item.physics.location.floor != floor) {
        continue;
      }

      const ItemData& data = *item.ref.data;

      auto it = m_data.atlases.find(data.sprite.atlas);

      if (it == m_data.atlases.end()) {
        gf::Log::error("Unknown atlas for item '%s'\n", data.name.c_str());
        continue;
      }

      const AtlasData& atlas = it->second;
      const gf::Texture& texture = m_resources.getTexture(atlas.path);

      gf::Vector2f textureSize = 1.0f / atlas.size;
      gf::Vector2i textureIndex = { data.sprite.index % atlas.size.width, data.sprite.index / atlas.size.width };

      gf::Sprite sprite(texture);
      sprite.setTextureRect(gf::RectF::fromPositionSize(textureSize * textureIndex, textureSize));
      sprite.setAnchor(gf::Anchor::Center);
      sprite.setPosition(item.physics.location.position);
      sprite.setRotation(item.physics.angle);
      sprite.setScale(data.sprite.scale);
      target.draw(sprite, states);
    }

  }

}

