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
#ifndef AKGR_PHYSICS_RUNTIME_H
#define AKGR_PHYSICS_RUNTIME_H

#include <map>
#include <vector>

#include <Box2D/Box2D.h>

#include <gf/Circ.h>
#include <gf/Entity.h>
#include <gf/Polygon.h>
#include <gf/Vector.h>

#include "Script.h"

namespace akgr {
    struct Zone;

    class PhysicsListener : public b2ContactListener {
    public:
      PhysicsListener(Script& script);

      virtual void BeginContact(b2Contact* contact) override;

    private:
      Script& m_script;
      std::map<b2Fixture*, const Zone*> m_zones;
    };

    struct PhysicsState;

    class PhysicsDebugger : public gf::Entity {
    public:
      PhysicsDebugger(PhysicsState& state)
      : gf::Entity(10000)
      , m_state(state)
      {

      }

      void setDebug(bool debug);

      virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

    private:
      PhysicsState& m_state;

      struct Polygon {
        gf::Polygon shape;
        gf::Color4f color;
      };

      struct Circle {
        gf::CircF shape;
        gf::Color4f color;
      };

      struct SolidCircle {
        gf::CircF shape;
        gf::Vector2f axis;
        gf::Color4f color;
      };

      struct Segment {
        gf::Vector2f p1;
        gf::Vector2f p2;
        gf::Color4f color;
      };

      struct Transform {
        gf::Vector2f position;
        gf::Vector2f xAxis;
        gf::Vector2f yAxis;
      };

      struct Point {
        gf::Vector2f position;
        float size;
        gf::Color4f color;
      };

      struct PhysicsDraw : public b2Draw {
      public:
        PhysicsDraw();

        /// Draw a closed polygon provided in CCW order.
        virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

        /// Draw a solid closed polygon provided in CCW order.
        virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

        /// Draw a circle.
        virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;

        /// Draw a solid circle.
        virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;

        /// Draw a line segment.
        virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

        /// Draw a transform. Choose your own length scale.
        /// @param xf a transform.
        virtual void DrawTransform(const b2Transform& xf) override;

        /// Draw a point.
        virtual void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);

        std::vector<Polygon> polygons;
        std::vector<Polygon> solidPolygons;
        std::vector<Circle> circles;
        std::vector<SolidCircle> solidCircles;
        std::vector<Segment> segments;
        std::vector<Transform> transforms;
        std::vector<Point> points;
      };

      PhysicsDraw m_draw;
    };


}

#endif // AKGR_PHYSICS_RUNTIME_H
