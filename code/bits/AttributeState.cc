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
#include "AttributeState.h"

#include <cassert>

namespace akgr {

  void AttributeValue::increase(int32_t gain, AttributeValue& anti1, AttributeValue& anti2) {
    gain += (gain % 2);
    int32_t loss = gain / 2;

    if (loss > anti1.value) {
      loss = anti1.value;
    }

    if (loss > anti2.value) {
      loss = anti2.value;
    }

    gain = loss * 2;

    value += gain;
    anti1.value -= loss;
    anti2.value -= loss;
  }

  void AttributeState::increase(Attribute kind, int32_t gain) {
    switch (kind) {
      case Attribute::Strength:
        strength.increase(gain, intelligence, wisdom);
        break;
      case Attribute::Dexterity:
        dexterity.increase(gain, wisdom, knowledge);
        break;
      case Attribute::Intelligence:
        intelligence.increase(gain, knowledge, strength);
        break;
      case Attribute::Wisdom:
        wisdom.increase(gain, strength, dexterity);
        break;
      case Attribute::Knowledge:
        knowledge.increase(gain, dexterity, intelligence);
        break;
    }
  }

  AttributeValue& AttributeState::operator[](Attribute attribute) {
    switch (attribute) {
      case Attribute::Strength:
        return strength;
      case Attribute::Dexterity:
        return dexterity;
      case Attribute::Intelligence:
        return intelligence;
      case Attribute::Wisdom:
        return wisdom;
      case Attribute::Knowledge:
        return knowledge;
    }

    assert(false);
    return strength;
  }

}
