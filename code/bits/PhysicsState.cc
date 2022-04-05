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
#include "PhysicsState.h"

#include <gf/Log.h>

#include "WorldData.h"

namespace akgr {

  namespace {

//     enum class PhysicsProperty {
//       Sensor = 0x01,
//     }

//     static constexpr float PhysicsScale = 0.02f;

//     static constexpr uint16_t THIRD_FLOOR_INSIDE       = 0x8000;
//     static constexpr uint16_t THIRD_FLOOR_OUTSIDE      = 0x4000;
//     static constexpr uint16_t SECOND_FLOOR_INSIDE      = 0x2000;
//     static constexpr uint16_t SECOND_FLOOR_OUTSIDE     = 0x1000;
//     static constexpr uint16_t FIRST_FLOOR_INSIDE       = 0x0800;
//     static constexpr uint16_t FIRST_FLOOR_OUTSIDE      = 0x0400;
//     static constexpr uint16_t GROUND_FLOOR_INSIDE      = 0x0200;
//     static constexpr uint16_t GROUND_FLOOR_OUTSIDE     = 0x0100;
//     static constexpr uint16_t FIRST_BASEMENT_INSIDE    = 0x0080;
//     static constexpr uint16_t FIRST_BASEMENT_OUTSIDE   = 0x0040;
//     static constexpr uint16_t SECOND_BASEMENT_INSIDE   = 0x0020;
//     static constexpr uint16_t SECOND_BASEMENT_OUTSIDE  = 0x0010;
//     static constexpr uint16_t THIRD_BASEMENT_INSIDE    = 0x0008;
//     static constexpr uint16_t THIRD_BASEMENT_OUTSIDE   = 0x0004;

    constexpr uint16_t bitsFromFloor(int32_t floor) {
      if (floor < -6 || floor > 7) {
        gf::Log::debug("Floor: %i\n", static_cast<int>(floor));
      }

      assert(-6 <= floor && floor <= 7);
      return static_cast<uint16_t>(1 << (floor + 8));
    }

    void updateFloorBits(b2Fixture *fixture, int32_t floor) {
      b2Filter filter = fixture->GetFilterData();
      filter.categoryBits = filter.maskBits = bitsFromFloor(floor);
      fixture->SetFilterData(filter);
    }

    void updateFloorBits(b2Body *body, int32_t floor) {
      for (b2Fixture *fixture = body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext()) {
        updateFloorBits(fixture, floor);
      }
    }


  }

  /*
   * PhysicsBody
   */

  void PhysicsBody::pullLocation() {
    assert(body);
    auto position = body->GetPosition();
    location.position.x = position.x / PhysicsScale;
    location.position.y = position.y / PhysicsScale;
    angle = body->GetAngle();
  }

  void PhysicsBody::pushLocation() {
    assert(body);
    body->SetTransform(b2Vec2(location.position.x * PhysicsScale, location.position.y * PhysicsScale), angle);
    updateFloorBits(body, location.floor);
  }

  void PhysicsBody::pushFloor() {
    assert(body);
    updateFloorBits(body, location.floor);
  }

  void PhysicsBody::setAngleAndVelocity(float newAngle, float newVelocity) {
    angle = newAngle;

    assert(body);
    body->SetTransform(body->GetPosition(), newAngle);
    body->SetAngularVelocity(0.0f);

    b2Rot rot(newAngle);
    b2Vec2 vel(0.0f, newVelocity * PhysicsScale);
    vel = b2Mul(rot, vel);
    body->SetLinearVelocity(vel);
  }

  /*
   * PhysicsState
   */

  PhysicsState::PhysicsState()
  : model(PhysicsScale, { 0.0f, 0.0f })
  {

  }

  void PhysicsState::update(gf::Time time) {
    model.update(time);
  }

  void PhysicsState::bind(const WorldData& data) {
    auto createCollisionFixture = [this](const Location& location, float angle, const gf::Polyline& line) {
      auto body = model.createSimpleBody(location.position, angle, gfb2d::BodyType::Static);
      auto fixture = model.createPolylineFixture(body, line);
      updateFloorBits(fixture, location.floor);
    };

    for (auto& collision : data.physics.collisions) {
      gf::Polyline line = collision.line;
      createCollisionFixture(collision.location, 0.0f, line);
      line.reverse();
      createCollisionFixture(collision.location, 0.0f, line);
    }

    for (auto& thing : data.physics.things) {
      createCollisionFixture(thing.location, gf::degreesToRadians(thing.angle), thing.line);
    }
  }

  b2Body *PhysicsState::createHeroBody(const Location& location, float angle) {
    static constexpr float HeroRadius = 35.0f;

    auto body = model.createSimpleBody(location.position, angle, gfb2d::BodyType::Dynamic);
    auto fixture = model.createCircleFixture(body, HeroRadius);
    updateFloorBits(fixture, location.floor);
    return body;
  }

  b2Body *PhysicsState::createCharacterBody(const Location& location, float angle) {
    static constexpr float CharacterWidth = 60.0f;
    static constexpr float CharacterHeight = 55.0f;

    auto body = model.createSimpleBody(location.position, angle, gfb2d::BodyType::Kinematic);
    auto fixture = model.createRectangleFixture(body, { CharacterWidth, CharacterHeight });
    updateFloorBits(fixture, location.floor);
    return body;
  }

  b2Body *PhysicsState::createItemBody(const Location& location, float angle, const Shape& shape) {
    auto body = model.createSimpleBody(location.position, angle, gfb2d::BodyType::Static);

    switch (shape.type) {
      case ShapeType::None:
        break;

      case ShapeType::Rectangle: {
        auto fixture = model.createRectangleFixture(body, { shape.rectangle.width, shape.rectangle.height });
        updateFloorBits(fixture, location.floor);
        break;
      }

      case ShapeType::Circle: {
        auto fixture = model.createCircleFixture(body, shape.circle.radius);
        updateFloorBits(fixture, location.floor);
        break;
      }
    }

    return body;
  }

  b2Fixture *PhysicsState::createFixtureForZone(const Zone& zone) {
    auto body = model.createSimpleBody(zone.location.position, 0.0f, gfb2d::BodyType::Static);
    auto fixture = model.createPolygonFixture(body, zone.polygon, gfb2d::FixtureFlags::Sensor);
    updateFloorBits(fixture, zone.location.floor);
    return fixture;
  }

}
