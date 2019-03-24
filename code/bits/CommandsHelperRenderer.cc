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
#include "CommandsHelperRenderer.h"

#include <gf/Coordinates.h>
#include <gf/Color.h>
#include <gf/Direction.h>
#include <gf/Gamepad.h>
#include <gf/Id.h>
#include <gf/Keyboard.h>
#include <gf/Log.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/RenderTarget.h>
#include <gf/Vector.h>
#include <gf/VectorOps.h>

using namespace gf::literals;

namespace akgr {

  namespace {

    constexpr gf::Vector2f HelperPosition(0.0f, 0.95f);
    constexpr gf::Vector2f HelperSize(0.3f, 0.05f);
    constexpr gf::Vector2f HelperFadeSize(0.1f, 0.05f);

    constexpr float HelperCharacterSize = 0.03f;

    constexpr gf::Vector2f HelperPadding(0.005f, 0.015f);
    constexpr float HelperLargeSpacing = 0.01f;
    constexpr float HelperSmallSpacing = 0.002f;

    constexpr gf::Vector2f HelperKeySize(0.015f, 0.035f);


    class FadedRectangle : public gf::Transformable {
    public:
      FadedRectangle(gf::Vector2f size, gf::Color4f color, gf::Direction direction)
      : m_size(size)
      , m_color(color)
      , m_direction(direction)
      {

      }

      virtual void draw(gf::RenderTarget& target, const gf::RenderStates& states) override {
        gf::Vertex vertices[4];

        vertices[0].position = { 0.0f,          0.0f          };
        vertices[1].position = { m_size.width,  0.0f          };
        vertices[2].position = { 0.0f,          m_size.height };
        vertices[3].position = { m_size.width,  m_size.height };

        if (m_direction == gf::Direction::Up || m_direction == gf::Direction::Left) {
          vertices[0].color = m_color;
        } else {
          vertices[0].color = gf::Color::Transparent;
        }

        if (m_direction == gf::Direction::Up || m_direction == gf::Direction::Right) {
          vertices[1].color = m_color;
        } else {
          vertices[1].color = gf::Color::Transparent;
        }

        if (m_direction == gf::Direction::Down || m_direction == gf::Direction::Left) {
          vertices[2].color = m_color;
        } else {
          vertices[2].color = gf::Color::Transparent;
        }

        if (m_direction == gf::Direction::Down || m_direction == gf::Direction::Right) {
          vertices[3].color = m_color;
        } else {
          vertices[3].color = gf::Color::Transparent;
        }

        gf::RenderStates localStates = states;

        localStates.transform *= getTransform();
        target.draw(vertices, 4, gf::PrimitiveType::TriangleStrip, localStates);
      }

    private:
      gf::Vector2f m_size;
      gf::Color4f m_color;
      gf::Direction m_direction;
    };

  } // anonymous namespace

  CommandsHelperRenderer::CommandsHelperRenderer(const UIData& data, const RootScenery& root, gf::ResourceManager& resources)
  : gf::Entity(50)
  , m_data(data)
  , m_root(root)
  , m_texture(resources.getTexture("pictures/icons.png"))
  , m_font(resources.getFont("fonts/Philosopher-Regular.ttf")) // fonts/DejaVuSans.ttf
  {

  }

  void CommandsHelperRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    if (m_root.helper.status == HelperStatus::None) {
      return;
    }

    gf::Coordinates coords(target);

    gf::Vector2f position = coords.getRelativePoint(HelperPosition);
    gf::Vector2f size = coords.getRelativeSize(HelperSize);
    gf::Vector2f padding = coords.getRelativeSize(HelperPadding);
    gf::Vector2f largeSpacing = coords.getRelativeSize({ HelperLargeSpacing, 0.0f });
    gf::Vector2f smallSpacing = coords.getRelativeSize({ HelperSmallSpacing, 0.0f });

    unsigned absoluteCharacterSize = coords.getRelativeCharacterSize(HelperCharacterSize);

    gf::RectangleShape background(size);
    background.setColor(gf::Color::Gray(0.1f) * gf::Color::Opaque(0.8f));
    background.setPosition(position);
    target.draw(background, states);

    FadedRectangle faded(coords.getRelativeSize(HelperFadeSize), gf::Color::Gray(0.1f) * gf::Color::Opaque(0.8f), gf::Direction::Left);
    faded.setPosition(position + gf::projx(size));
    target.draw(faded, states);

    gf::Sprite sprite(m_texture);

    if (m_root.options.data.input == OptionsData::Input::Gamepad) {
      sprite.setTextureRect(gf::RectF({ 0.500f, 0.0f }, { 0.125f, 0.125f }));
    } else {
      sprite.setTextureRect(gf::RectF({ 0.375f, 0.0f }, { 0.125f, 0.125f }));
    }

    sprite.setPosition(position + gf::projx(padding) / 2);
    sprite.setScale(size.height / 64.0f);
    sprite.setAnchor(gf::Anchor::TopLeft);
    target.draw(sprite, states);

    position += gf::dirx(size.height) + gf::projx(padding);

    //

    gf::Vector2f keySize = coords.getRelativeSize(HelperKeySize);

    auto drawKey = [&,this](std::string name) {
      gf::Vector2f keyPosition = position + gf::projx(keySize / 2) + gf::projy(size / 2);

      gf::RectangleShape shape(keySize);
      shape.setColor(gf::Color::Gray(0.2f) * gf::Color::Opaque(0.8f));
      shape.setPosition(keyPosition);
      shape.setAnchor(gf::Anchor::Center);
      target.draw(shape, states);

      gf::Text text(name, m_font, absoluteCharacterSize);
      text.setColor(gf::Color::White);
      text.setPosition(keyPosition);
      text.setAnchor(gf::Anchor::Center);
      target.draw(text, states);

      position += gf::projx(keySize);
    };

    auto drawButton = [&,this](std::string name) {
      float buttonSize = size.height;
      gf::Vector2f buttonPosition = position + gf::dirx(buttonSize / 2) + gf::projy(size / 2);

      gf::CircleShape shape(keySize.height / 2);
      shape.setColor(gf::Color::Gray(0.2f) * gf::Color::Opaque(0.8f));
      shape.setPosition(buttonPosition);
      shape.setAnchor(gf::Anchor::Center);
      target.draw(shape, states);

      gf::Text text(name, m_font, absoluteCharacterSize);
      text.setColor(gf::Color::White);
      text.setPosition(buttonPosition);
      text.setAnchor(gf::Anchor::Center);
      target.draw(text, states);

      position += gf::dirx(buttonSize);
    };

    auto drawText = [&,this](std::string content) {
      gf::Text text(content, m_font, absoluteCharacterSize);
      text.setColor(gf::Color::White);
      text.setPosition(position + padding);
      text.setAnchor(gf::Anchor::TopLeft);
      target.draw(text, states);

      position += gf::projx(text.getLocalBounds().getSize()) + gf::projx(padding * 2);
    };

    position += largeSpacing;

    switch (m_root.helper.status) {
      case HelperStatus::Continue:
        switch (m_root.options.data.input) {
          case OptionsData::Input::Gamepad:
            drawButton("A");
            drawText(m_data.getUIMessage("HelperContinue"_id));
            break;
          case OptionsData::Input::Keyboard:
            drawKey(gf::Keyboard::getKeycodeName(gf::Keyboard::localize(gf::Scancode::E)));
            position += smallSpacing;
            drawText(m_data.getUIMessage("HelperContinue"_id));
            break;
        }
        break; // HelperStatus::Continue

      case HelperStatus::Menu:
        switch (m_root.options.data.input) {
          case OptionsData::Input::Gamepad:
            drawButton("A");
            drawText(m_data.getUIMessage("HelperSelect"_id));
//             position += largeSpacing;
//             drawButton("B");
//             drawText("Fight");
            break;
          case OptionsData::Input::Keyboard:
            drawKey(gf::Keyboard::getKeycodeName(gf::Keyboard::localize(gf::Scancode::E)));
            position += smallSpacing;
            drawText(m_data.getUIMessage("HelperSelect"_id));
//             position += largeSpacing;
//             drawKey(gf::Keyboard::getKeycodeName(gf::Keyboard::localize(gf::Scancode::F)));
//             position += smallSpacing;
//             drawText("Fight");
            break;
        }
        break; // HelperStatus::Menu

      case HelperStatus::Talk:
        switch (m_root.options.data.input) {
          case OptionsData::Input::Gamepad:
            drawButton("A");
            drawText(m_data.getUIMessage("HelperTalk"_id));
            break;
          case OptionsData::Input::Keyboard:
            drawKey(gf::Keyboard::getKeycodeName(gf::Keyboard::localize(gf::Scancode::E)));
            position += smallSpacing;
            drawText(m_data.getUIMessage("HelperTalk"_id));
            break;
        }
        break; // HelperStatus::Talk

      case HelperStatus::Use:
        switch (m_root.options.data.input) {
          case OptionsData::Input::Gamepad:
            drawButton("A");
            drawText(m_data.getUIMessage("HelperUse"_id));
            break;
          case OptionsData::Input::Keyboard:
            drawKey(gf::Keyboard::getKeycodeName(gf::Keyboard::localize(gf::Scancode::E)));
            position += smallSpacing;
            drawText(m_data.getUIMessage("HelperUse"_id));
            break;
        }
        break; // HelperStatus::Use

      default:
        break;
    }


  }

}
