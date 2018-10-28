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
#include "PhysicsRuntime.h"

#include <gf/Curves.h>
#include <gf/Shapes.h>
#include <gf/RenderTarget.h>

#include <gf/Log.h>

#include "WorldData.h"
#include "WorldState.h"

namespace akgr {

  PhysicsListener::PhysicsListener(Script& script)
  : m_script(script)
  {

    for (auto& zone : script.getData().physics.zones) {
      auto fixture = script.getState().physics.createFixtureForZone(zone);
      m_zones.insert(std::make_pair(fixture, &zone));
    }

    script.getState().physics.world.SetContactListener(this);
  }


  void PhysicsListener::BeginContact(b2Contact* contact) {
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    for (auto fixture : { fixtureA, fixtureB }) {
      auto it = m_zones.find(fixture);

      if (it == m_zones.end()) {
        continue;
      }

      const Zone *zone = it->second;
      auto& requirements = m_script.getState().hero.requirements;

      if (std::includes(requirements.begin(), requirements.end(), zone->requirements.begin(), zone->requirements.end())) {
        m_script.onMessageDefered(zone->message);
      }
    }
  }

  /*
   * PhysicsDebugger
   */

  namespace {
    constexpr float DebugOutlineThickness = 1.0f;
    constexpr float DebugTransformLength = 0.5f;
  }

  void PhysicsDebugger::addPolygon(gf::Polygon polygon, gf::Color4f color) {
    m_polygons.push_back({ std::move(polygon), color });
  }

  void PhysicsDebugger::addSolidPolygon(gf::Polygon polygon, gf::Color4f color) {
    m_solidPolygons.push_back({ std::move(polygon), color });
  }

  void PhysicsDebugger::addCircle(gf::CircF circle, gf::Color4f color) {
    m_circles.push_back({ circle, color });
  }

  void PhysicsDebugger::addSolidCircle(gf::CircF circle, gf::Vector2f axis, gf::Color4f color) {
    m_solidCircles.push_back({ circle, axis, color });
  }

  void PhysicsDebugger::addSegment(gf::Vector2f p1, gf::Vector2f p2, gf::Color4f color) {
    m_segments.push_back({ p1, p2, color });
  }

  void PhysicsDebugger::addTransform(gf::Vector2f position, gf::Vector2f xAxis, gf::Vector2f yAxis) {
    m_transforms.push_back({ position, xAxis, yAxis });
  }

  void PhysicsDebugger::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    m_state.world.DrawDebugData();

    for (auto& polygon : m_polygons) {
      gf::ConvexShape shape(polygon.shape);
      shape.setColor(gf::Color::Transparent);
      shape.setOutlineColor(polygon.color);
      shape.setOutlineThickness(DebugOutlineThickness);
      target.draw(shape, states);
    }

    for (auto& polygon : m_solidPolygons) {
      gf::ConvexShape shape(polygon.shape);
      shape.setColor(polygon.color);
      target.draw(shape, states);
    }

    for (auto& circle : m_circles) {
      gf::CircleShape shape(circle.shape);
      shape.setColor(gf::Color::Transparent);
      shape.setOutlineColor(circle.color);
      shape.setOutlineThickness(DebugOutlineThickness);
      target.draw(shape, states);
    }

    for (auto& circle : m_solidCircles) {
      gf::CircleShape shape(circle.shape);
      shape.setColor(circle.color);
      target.draw(shape, states);

      gf::Line line(circle.shape.center, circle.shape.center + circle.axis);
      line.setWidth(2.5f * DebugOutlineThickness);
      line.setColor(gf::Color::darker(circle.color, 0.3f));
      target.draw(line, states);
    }

    for (auto& segment : m_segments) {
      gf::Line curve(segment.p1, segment.p2);
      curve.setWidth(DebugOutlineThickness);
      curve.setColor(segment.color);
      target.draw(curve, states);
    }

    for (auto& transform : m_transforms) {
      gf::Line lineX(transform.position, transform.position + DebugTransformLength * transform.xAxis);
      lineX.setWidth(DebugOutlineThickness);
      lineX.setColor(gf::Color::Red);
      target.draw(lineX, states);

      gf::Line lineY(transform.position, transform.position + DebugTransformLength * transform.yAxis);
      lineY.setWidth(DebugOutlineThickness);
      lineY.setColor(gf::Color::Green);
      target.draw(lineY, states);
    }

    m_polygons.clear();
    m_solidPolygons.clear();
    m_circles.clear();
    m_solidCircles.clear();
    m_segments.clear();
    m_transforms.clear();
  }


  namespace {

    gf::Vector2f toVec(b2Vec2 vec) {
      return { vec.x / PhysicsScale, vec.y / PhysicsScale };
    }

    gf::Color4f toColor(b2Color color) {
      return { color.r, color.g, color.b, color.a * 0.8f };
    }

  }

  PhysicsDraw::PhysicsDraw(PhysicsDebugger& debug)
  : m_debug(debug)
  {
    SetFlags(b2Draw::e_shapeBit /* | b2Draw::e_aabbBit */ | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit);
  }

  void PhysicsDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    gf::Polygon polygon;

    for (int32 i = 0; i < vertexCount; ++i) {
      polygon.addPoint(toVec(vertices[i]));
    }

    m_debug.addPolygon(std::move(polygon), toColor(color));
  }

  void PhysicsDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    gf::Polygon polygon;

    for (int32 i = 0; i < vertexCount; ++i) {
      polygon.addPoint(toVec(vertices[i]));
    }

    m_debug.addSolidPolygon(std::move(polygon), toColor(color));
  }

  void PhysicsDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
    m_debug.addCircle(gf::CircF(toVec(center), radius / PhysicsScale), toColor(color));
  }

  void PhysicsDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
    m_debug.addSolidCircle(gf::CircF(toVec(center), radius / PhysicsScale), toVec(axis), toColor(color));
  }

  void PhysicsDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
    m_debug.addSegment(toVec(p1), toVec(p2), toColor(color));
  }

  void PhysicsDraw::DrawTransform(const b2Transform& xf) {
    m_debug.addTransform(toVec(xf.p), toVec(xf.q.GetXAxis()), toVec(xf.q.GetYAxis()));
  }

}
