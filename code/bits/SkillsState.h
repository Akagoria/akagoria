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
#ifndef AKGR_SKILLS_STATE_H
#define AKGR_SKILLS_STATE_H

#include <cstdint>

namespace akgr {
  enum class SkillKind {
    Strength,
    Dexterity,
    Intelligence,
    Wisdom,
    Knowledge,
  };

  struct Skill {
    int32_t value = 8000;

    void increase(int32_t gain, int32_t extra, Skill& anti1, Skill& anti2) {
      gain += (gain % 2);
      int32_t loss = gain / 2;

      if (loss > anti1.value) {
        loss = anti1.value;
      }

      if (loss > anti2.value) {
        loss = anti2.value;
      }

      gain = loss * 2 + extra;

      value += gain;
      anti1.value -= loss;
      anti1.value -= loss;
    }
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, Skill& skill) {
    return ar | skill.value;
  }

  struct SkillsState {
    Skill strength;
    Skill dexterity;
    Skill intelligence;
    Skill wisdom;
    Skill knowledge;

    void increase(SkillKind kind, int32_t gain, int32_t extra) {
      switch (kind) {
        case SkillKind::Strength:
          strength.increase(gain, extra, intelligence, wisdom);
          break;
        case SkillKind::Dexterity:
          dexterity.increase(gain, extra, wisdom, knowledge);
          break;
        case SkillKind::Intelligence:
          intelligence.increase(gain, extra, knowledge, strength);
          break;
        case SkillKind::Wisdom:
          wisdom.increase(gain, extra, strength, dexterity);
          break;
        case SkillKind::Knowledge:
          knowledge.increase(gain, extra, dexterity, intelligence);
          break;
      }
    }

    Skill& getSkill(SkillKind kind) {
      switch (kind) {
        case SkillKind::Strength:
          return strength;
        case SkillKind::Dexterity:
          return dexterity;
        case SkillKind::Intelligence:
          return intelligence;
        case SkillKind::Wisdom:
          return wisdom;
        case SkillKind::Knowledge:
          return knowledge;
      }
    }
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, SkillsState& state) {
    return ar | state.strength | state.dexterity | state.intelligence | state.wisdom | state.knowledge;
  }

}

#endif // AKGR_SKILLS_STATE_H
