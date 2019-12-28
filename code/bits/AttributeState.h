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
#ifndef AKGR_ATTRIBUTE_STATE_H
#define AKGR_ATTRIBUTE_STATE_H

#include <cstdint>

#include "Attribute.h"
#include "Value.h"

namespace akgr {

  struct AttributeValue {
    Value value = 50;

    void increase(Value gain, AttributeValue& anti1, AttributeValue& anti2);
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, AttributeValue& attribute) {
    return ar | attribute.value;
  }

  struct AttributeState {
    AttributeValue strength;
    AttributeValue dexterity;
    AttributeValue intelligence;
    AttributeValue wisdom;
    AttributeValue knowledge;

    void increase(Attribute kind, Value gain);
    AttributeValue& operator[](Attribute attribute);
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, AttributeState& state) {
    return ar | state.strength | state.dexterity | state.intelligence | state.wisdom | state.knowledge;
  }

}

#endif // AKGR_ATTRIBUTE_STATE_H
