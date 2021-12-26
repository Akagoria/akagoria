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
#ifndef AKGR_DICT_H
#define AKGR_DICT_H

#include <cassert>
#include <algorithm>
#include <vector>

namespace akgr {

  template<typename T>
  using Dict = std::vector<T>;

  template<typename T>
  struct DictCompare {
    bool operator()(const T& lhs, const T& rhs) {
      return lhs.name.id < rhs.name.id;
    }

    bool operator()(const T& lhs, gf::Id rhs) {
      return lhs.name.id < rhs;
    }

    bool operator()(gf::Id lhs, const T& rhs) {
      return lhs < rhs.name.id;
    }
  };

  template<typename T>
  inline
  void dictSort(Dict<T>& dict) {
    std::sort(dict.begin(), dict.end(), DictCompare<T>{});
  }

  template<typename T>
  inline
  const T *dictFind(const Dict<T>& dict, gf::Id id) {
    auto [ begin, end ] = std::equal_range(dict.begin(), dict.end(), id, DictCompare<T>{});

    if (begin == end) {
      return nullptr;
    }

    assert(end - begin == 1);
    return std::addressof(*begin);
  }

}

#endif // AKGR_DICT_H
