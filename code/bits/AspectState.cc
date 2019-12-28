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
#include "AspectState.h"

#include <cassert>

namespace akgr {

  bool AspectValue::increase(gf::Time time) {
    value += (max / 20) * time.asSeconds();

    if (value > max) {
      value = max;
      return false;
    }

    return true;
  }

  void AspectValue::update(gf::Time time, gf::Time maxPeriod) {
    period += time;

    while (period > maxPeriod) {
      if (value < max) {
        value += 1;
      } else {
        value = max;
      }

      period -= maxPeriod;
    }
  }

  AspectValue& AspectState::operator[](Aspect aspect) {
    switch (aspect) {
      case Aspect::Health:
        return hp;
      case Aspect::Magic:
        return mp;
      case Aspect::Vitality:
        return vp;
    }

    assert(false);
    return hp;
  }

}
