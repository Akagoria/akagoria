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
#ifndef AKGR_SHAPE_H
#define AKGR_SHAPE_H

#include <gf/Vector.h>

namespace akgr {

  enum class ShapeType : uint8_t {
    None,
    Circle,
    Rectangle,
  };

  struct Shape {
    ShapeType type;

    struct Circ {
      float radius;
    };

    struct Rect {
      float width;
      float height;
    };

    union {
      Circ circle;
      Rect rectangle;
    };

    float getPhysicalSize() const {
      switch (type) {
        case ShapeType::None:
          return 0;
        case ShapeType::Circle:
          return circle.radius;
        case ShapeType::Rectangle:
          return std::min(rectangle.width, rectangle.height);
      }

      return 0;
    }
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, Shape& data) {
    ar | data.type;

    switch (data.type) {
      case ShapeType::None:
        break;
      case ShapeType::Circle:
        ar | data.circle.radius;
        break;
      case ShapeType::Rectangle:
        ar | data.rectangle.width | data.rectangle.height;
        break;
    }

    return ar;
  }

}

#endif // AKGR_SHAPE_H
