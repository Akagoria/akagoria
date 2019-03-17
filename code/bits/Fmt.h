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
#ifndef AKGR_FMT_H
#define AKGR_FMT_H

#include <fmt/format.h>

#include <gf/Id.h>
#include <gf/Rect.h>
#include <gf/Vector.h>

#include "LocationData.h"

struct Id {
  gf::Id value;
};


namespace fmt {
  template <>
  struct formatter<gf::Vector2f> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const gf::Vector2f& vec, FormatContext& ctx) {
      return format_to(ctx.begin(), "({:g}, {:g})", vec.x, vec.y);
    }
  };

  template <>
  struct formatter<gf::Vector2u> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const gf::Vector2u& vec, FormatContext& ctx) {
      return format_to(ctx.begin(), "({}, {})", vec.x, vec.y);
    }
  };

  template <>
  struct formatter<gf::RectU> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const gf::RectU& rect, FormatContext& ctx) {
      return format_to(ctx.begin(), "[{}, {}]", rect.getPosition(), rect.getSize());
    }
  };

  template <>
  struct formatter<akgr::Location> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const akgr::Location& loc, FormatContext& ctx) {
      return format_to(ctx.begin(), "{}[{}]", loc.position, loc.floor);
    }
  };

  template <>
  struct formatter<Id> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const Id& id, FormatContext& ctx) {
      return format_to(ctx.begin(), "{:X}", id.value);
    }
  };

}

#endif // AKGR_FMT_H
