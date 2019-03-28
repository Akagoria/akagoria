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
#ifndef AKGR_PHYSICS_STATE_H
#define AKGR_PHYSICS_STATE_H

#include <memory>

#include <Box2D/Box2D.h>

#include <gf/Time.h>

#include "LocationTypes.h"
#include "Shape.h"

namespace akgr {
  struct WorldData;
  struct Zone;

  struct PhysicsBody {
    Location location;
    float angle = 0.0f;
    b2Body *body;

    void pullLocation();
    void pushLocation();
    void pushFloor();

    void setAngleAndVelocity(float newAngle, float newVelocity);
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, PhysicsBody& body) {
    return ar | body.location | body.angle;
  }

  struct PhysicsState {
    PhysicsState();
    ~PhysicsState();

    void update(gf::Time time);
    void bind(const WorldData& data);

    b2Body *createHeroBody(const Location& location, float angle);
    b2Body *createCharacterBody(const Location& location, float angle);
    b2Body *createItemBody(const Location& location, float angle, const Shape& shape);

    b2Fixture *createFixtureForZone(const Zone& zone);

    b2World world;
  };

}

#endif // AKGR_PHYSICS_STATE_H
