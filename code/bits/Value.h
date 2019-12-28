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
#ifndef AKGR_VALUE_H
#define AKGR_VALUE_H

#include <cstdint>

#include <gf/Types.h>

namespace akgr {

  struct Value {
    constexpr
    Value()
    : data(0)
    {
    }

    constexpr
    Value(int32_t val)
    : data(val * Factor)
    {
    }

    constexpr
    Value(float val)
    : data(static_cast<int32_t>(val * Factor))
    {
    }

    constexpr
    int32_t asInt() const {
      return data / Factor;
    }

    constexpr
    float asFloat() const {
      return static_cast<float>(data) / static_cast<float>(Factor);
    }

    constexpr
    Value& operator+=(Value other) {
      data += other.data;
      return *this;
    }

    constexpr
    Value& operator-=(Value other) {
      data -= other.data;
      return *this;
    }

    constexpr
    Value& operator*=(Value other) {
      data *= other.data;
      data /= Factor;
      return *this;
    }

    constexpr
    Value& operator/=(Value other) {
      data *= Factor;
      data /= other.data;
      return *this;
    }

    constexpr
    Value& operator%=(Value other) {
      data %= other.data;
      return *this;
    }

    static constexpr int32_t Factor = 1000;
    int32_t data;
  };

  constexpr
  Value operator+(Value lhs, Value rhs) { return lhs += rhs; }

  constexpr
  Value operator-(Value lhs, Value rhs) { return lhs -= rhs; }

  constexpr
  Value operator*(Value lhs, Value rhs) { return lhs *= rhs; }

  constexpr
  Value operator/(Value lhs, Value rhs) { return lhs /= rhs; }

  constexpr
  Value operator%(Value lhs, Value rhs) { return lhs %= rhs; }

  constexpr
  bool operator==(Value lhs, Value rhs) { return lhs.data == rhs.data; }

  constexpr
  bool operator!=(Value lhs, Value rhs) { return lhs.data != rhs.data; }

  constexpr
  bool operator<(Value lhs, Value rhs) { return lhs.data < rhs.data; }

  constexpr
  bool operator<=(Value lhs, Value rhs) { return lhs.data <= rhs.data; }

  constexpr
  bool operator>(Value lhs, Value rhs) { return lhs.data > rhs.data; }

  constexpr
  bool operator>=(Value lhs, Value rhs) { return lhs.data >= rhs.data; }


  template<typename Archive>
  Archive& operator|(Archive& ar, Value& val) {
    return ar | val.data;
  }

}

#endif // AKGR_VALUE_H
