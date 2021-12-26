#ifndef AKGR_HERO_DATA_H
#define AKGR_HERO_DATA_H

#include "AtlasData.h"
#include "Dict.h"

namespace akgr {

  struct HeroData {
    Dict<AtlasAnimation> animations;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, HeroData& data) {
    return ar | data.animations;
  }

}

#endif // AKGR_HERO_DATA_H
