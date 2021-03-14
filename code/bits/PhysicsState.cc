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

    void updateFloorBits(b2Body *body, int32_t floor) {
      uint16_t floorBits = bitsFromFloor(floor);

      for (b2Fixture *fixture = body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext()) {
        b2Filter filter = fixture->GetFilterData();
        filter.categoryBits = floorBits;
        filter.maskBits = floorBits;
        fixture->SetFilterData(filter);
      }
    }

    template<typename T>
    b2Fixture *createFixture(b2World& world, const Location& location, float angle, T& shape, b2BodyType type, bool isSensor) {
      b2BodyDef bodyDef;
      bodyDef.type = type;
      bodyDef.position = { location.position.x * PhysicsScale, location.position.y * PhysicsScale };
      bodyDef.angle = angle;
      auto body = world.CreateBody(&bodyDef);

      b2FixtureDef fixtureDef;
      fixtureDef.isSensor = isSensor;
      fixtureDef.filter.categoryBits = fixtureDef.filter.maskBits = bitsFromFloor(location.floor);
      fixtureDef.density = 1.0f;
      fixtureDef.friction = 0.0f;
      fixtureDef.restitution = 0.0f;
      fixtureDef.shape = &shape;

      return body->CreateFixture(&fixtureDef);
    }

    void createPolylineFixture(b2World& world, const Location& location, float angle, const gf::Polyline& polyline, bool isSensor) {
      std::vector<b2Vec2> line;

      for (auto& point : polyline) {
        line.emplace_back(point.x * PhysicsScale, point.y * PhysicsScale);
      }

      b2ChainShape shape;
      if (polyline.isLoop()) {
        shape.CreateLoop(line.data(), line.size());
      } else {
        assert(polyline.isChain());
        shape.CreateChain(line.data(), line.size(), line.front(), line.back());
      }

      createFixture(world, location, angle, shape, b2_staticBody, isSensor);
    }

    b2Fixture *createPolygonFixture(b2World& world, const Location& location, float angle, const gf::Polygon& polygon, bool isSensor) {
      std::vector<b2Vec2> line;

      for (auto& point : polygon) {
        line.emplace_back(point.x * PhysicsScale, point.y * PhysicsScale);
      }

      b2PolygonShape shape;
      shape.Set(line.data(), line.size());
      return createFixture(world, location, angle, shape, b2_staticBody, isSensor);
    }

//     b2Fixture *createCircleFixture(b2World& world, const Location& location, float radius) {
//       b2CircleShape shape;
//       shape.m_radius = radius * PhysicsScale;
//       return createFixture(world, location, shape, b2_staticBody, /* isSensor */ false);
//     }
//
//     b2Fixture *createRectangleFixture(b2World& world, const Location& location, float width, float height) {
//       b2PolygonShape shape;
//       shape.SetAsBox(width * PhysicsScale * 0.5f, height * PhysicsScale * 0.5f);
//       return createFixture(world, location, shape, b2_staticBody, /* isSensor */ false);
//     }

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
  : world({ 0.0f, 0.0f })
  {

  }

  PhysicsState::~PhysicsState() = default;

  void PhysicsState::update(gf::Time time) {
    static constexpr int32 VelocityIterations = 10; // 6;
    static constexpr int32 PositionIterations = 8; // 2;
    world.Step(time.asSeconds(), VelocityIterations, PositionIterations);
  }

  void PhysicsState::bind(const WorldData& data) {
    for (auto& collision : data.physics.collisions) {
      createPolylineFixture(world, collision.location, 0.0f, collision.line, /* isSensor */ false);
    }

    for (auto& thing : data.physics.things) {
      createPolylineFixture(world, thing.location, gf::degreesToRadians(thing.angle), thing.line, /* isSensor */ false);
    }
  }

  b2Body *PhysicsState::createHeroBody(const Location& location, float angle) {
//     static constexpr float HeroWidth = 60.0f;
//     static constexpr float HeroHeight = 55.0f;

    static constexpr float HeroRadius = 35.0f;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // TODO: or b2_kinematicBody?
    bodyDef.position = { location.position.x * PhysicsScale, location.position.y * PhysicsScale };
    bodyDef.angle = angle;
    auto body = world.CreateBody(&bodyDef);

//     b2PolygonShape shape;
//     shape.SetAsBox(HeroWidth * PhysicsScale * 0.5f, HeroHeight * PhysicsScale * 0.5f);
    b2CircleShape shape;
    shape.m_radius = HeroRadius * PhysicsScale;

    b2FixtureDef fixtureDef;
    fixtureDef.isSensor = false;
    fixtureDef.filter.categoryBits = fixtureDef.filter.maskBits = bitsFromFloor(location.floor);
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.shape = &shape;
    body->CreateFixture(&fixtureDef);

    return body;
  }

  b2Body *PhysicsState::createCharacterBody(const Location& location, float angle) {
    static constexpr float CharacterWidth = 60.0f;
    static constexpr float CharacterHeight = 55.0f;

    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.position = { location.position.x * PhysicsScale, location.position.y * PhysicsScale };
    bodyDef.angle = angle;
    auto body = world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(CharacterWidth * PhysicsScale * 0.5f, CharacterHeight * PhysicsScale * 0.5f);

    b2FixtureDef fixtureDef;
    fixtureDef.isSensor = false;
    fixtureDef.filter.categoryBits = fixtureDef.filter.maskBits = bitsFromFloor(location.floor);
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.shape = &shape;
    body->CreateFixture(&fixtureDef);

    return body;
  }

  b2Body *PhysicsState::createItemBody(const Location& location, float angle, const Shape& shape) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = { location.position.x * PhysicsScale, location.position.y * PhysicsScale };
    bodyDef.angle = angle;
    auto body = world.CreateBody(&bodyDef);

//     b2PolygonShape shape;
//     shape.SetAsBox(CharacterWidth * PhysicsScale * 0.5f, CharacterHeight * PhysicsScale * 0.5f);

    b2FixtureDef fixtureDef;
    fixtureDef.isSensor = false;
    fixtureDef.filter.categoryBits = fixtureDef.filter.maskBits = bitsFromFloor(location.floor);
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;

    switch (shape.type) {
      case ShapeType::None:
        break;

      case ShapeType::Rectangle: {
        b2PolygonShape polygon;
        polygon.SetAsBox(shape.rectangle.width * PhysicsScale * 0.5f, shape.rectangle.height * PhysicsScale * 0.5f);
        fixtureDef.shape = &polygon;
        body->CreateFixture(&fixtureDef);
        break;
      }

      case ShapeType::Circle: {
        b2CircleShape circle;
        circle.m_radius = shape.circle.radius * PhysicsScale;
        fixtureDef.shape = &circle;
        body->CreateFixture(&fixtureDef);
        break;
      }
    }

    return body;
  }

  b2Fixture *PhysicsState::createFixtureForZone(const Zone& zone) {
    return createPolygonFixture(world, zone.location, 0.0f, zone.polygon, /* isSensor */ true);
  }

}
