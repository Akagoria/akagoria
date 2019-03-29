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

  void PhysicsDebugger::setDebug(bool debug) {
    m_state.world.SetDebugDraw(debug ? &m_draw : nullptr);
  }

  void PhysicsDebugger::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    m_state.world.DrawDebugData();

    for (auto& polygon : m_draw.polygons) {
      gf::ConvexShape shape(polygon.shape);
      shape.setColor(gf::Color::Transparent);
      shape.setOutlineColor(polygon.color);
      shape.setOutlineThickness(DebugOutlineThickness);
      target.draw(shape, states);
    }

    for (auto& polygon : m_draw.solidPolygons) {
      gf::ConvexShape shape(polygon.shape);
      shape.setColor(polygon.color);
      target.draw(shape, states);
    }

    for (auto& circle : m_draw.circles) {
      gf::CircleShape shape(circle.shape);
      shape.setColor(gf::Color::Transparent);
      shape.setOutlineColor(circle.color);
      shape.setOutlineThickness(DebugOutlineThickness);
      target.draw(shape, states);
    }

    for (auto& circle : m_draw.solidCircles) {
      gf::CircleShape shape(circle.shape);
      shape.setColor(circle.color);
      target.draw(shape, states);

      gf::Line line(circle.shape.center, circle.shape.center + circle.axis);
      line.setWidth(2.5f * DebugOutlineThickness);
      line.setColor(gf::Color::darker(circle.color, 0.3f));
      target.draw(line, states);
    }

    for (auto& segment : m_draw.segments) {
      gf::Line curve(segment.p1, segment.p2);
      curve.setWidth(DebugOutlineThickness);
      curve.setColor(segment.color);
      target.draw(curve, states);
    }

    for (auto& transform : m_draw.transforms) {
      gf::Line lineX(transform.position, transform.position + DebugTransformLength * transform.xAxis);
      lineX.setWidth(DebugOutlineThickness);
      lineX.setColor(gf::Color::Red);
      target.draw(lineX, states);

      gf::Line lineY(transform.position, transform.position + DebugTransformLength * transform.yAxis);
      lineY.setWidth(DebugOutlineThickness);
      lineY.setColor(gf::Color::Green);
      target.draw(lineY, states);
    }

    for (auto& point : m_draw.points) {
      gf::CircleShape circle(point.size / 2);
      circle.setColor(point.color);
      circle.setAnchor(gf::Anchor::Center);
      circle.setPosition(point.position);
      target.draw(circle, states);
    }

    m_draw.polygons.clear();
    m_draw.solidPolygons.clear();
    m_draw.circles.clear();
    m_draw.solidCircles.clear();
    m_draw.segments.clear();
    m_draw.transforms.clear();
    m_draw.points.clear();
  }


  namespace {

    gf::Vector2f toVec(b2Vec2 vec) {
      return { vec.x / PhysicsScale, vec.y / PhysicsScale };
    }

    gf::Color4f toColor(b2Color color) {
      return { color.r, color.g, color.b, color.a * 0.8f };
    }

  }

  PhysicsDebugger::PhysicsDraw::PhysicsDraw()
  {
    SetFlags(b2Draw::e_shapeBit /* | b2Draw::e_aabbBit */ | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit);
  }

  void PhysicsDebugger::PhysicsDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    gf::Polygon polygon;

    for (int32 i = 0; i < vertexCount; ++i) {
      polygon.addPoint(toVec(vertices[i]));
    }

    polygons.push_back({ std::move(polygon), toColor(color) });
  }

  void PhysicsDebugger::PhysicsDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    gf::Polygon polygon;

    for (int32 i = 0; i < vertexCount; ++i) {
      polygon.addPoint(toVec(vertices[i]));
    }

    solidPolygons.push_back({ std::move(polygon), toColor(color) });
  }

  void PhysicsDebugger::PhysicsDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
    circles.push_back({ gf::CircF(toVec(center), radius / PhysicsScale), toColor(color) });
  }

  void PhysicsDebugger::PhysicsDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
    solidCircles.push_back({ gf::CircF(toVec(center), radius / PhysicsScale), toVec(axis), toColor(color) });
  }

  void PhysicsDebugger::PhysicsDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
    segments.push_back({ toVec(p1), toVec(p2), toColor(color) });
  }

  void PhysicsDebugger::PhysicsDraw::DrawTransform(const b2Transform& xf) {
    transforms.push_back({ toVec(xf.p), toVec(xf.q.GetXAxis()), toVec(xf.q.GetYAxis()) });
  }

  void PhysicsDebugger::PhysicsDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) {
    points.push_back({ toVec(p), size, toColor(color) });
  }
}
