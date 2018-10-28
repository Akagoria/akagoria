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
#include "CharacterRenderer.h"

#include <gf/Color.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>

namespace akgr {

  namespace {

    constexpr float CharacterDialogRadius = 30.0f;
    constexpr float CharacterDialogThickness = 5.0f;

  }

  CharacterRenderer::CharacterRenderer(const WorldData& data, const WorldState& state)
  : m_data(data)
  , m_state(state)
  {

  }

  void CharacterRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    for (auto& character : m_state.characters) {
      if (character.physics.location.floor != m_state.hero.physics.location.floor) {
        continue;
      }

      if (character.dialog != gf::InvalidId) {
        DataRef<DialogData> dialogRef = { };
        dialogRef.id = character.dialog;
        dialogRef.bind(m_data.dialogs);
        assert(dialogRef.data);

        auto type = dialogRef.data->type;

        gf::Color4f color = gf::Color::Opaque(0.5f);

        switch (type) {
          case DialogData::Simple:
            color *= gf::Color::Red;
            break;
          case DialogData::Quest:
            color *= gf::Color::Yellow;
            break;
          case DialogData::Story:
            color *= gf::Color::Cyan;
            break;
        }

        gf::CircleShape shape(CharacterDialogRadius);
        shape.setColor(gf::Color::Transparent);
        shape.setOutlineThickness(CharacterDialogThickness);
        shape.setOutlineColor(color);
        shape.setAnchor(gf::Anchor::Center);
        shape.setPosition(character.physics.location.position);
        target.draw(shape, states);
      }

      gf::RectangleShape shape(character.ref.data->size);
      shape.setColor(gf::Color::Gray());
      shape.setOutlineColor(gf::Color::Gray(0.2f));
      shape.setOutlineThickness(2.0f);
      shape.setAnchor(gf::Anchor::Center);
      shape.setPosition(character.physics.location.position);
      shape.setRotation(character.physics.angle);
      target.draw(shape, states);
    }
  }

}

