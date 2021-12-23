#ifndef AKGR_HERO_DATA_H
#define AKGR_HERO_DATA_H

#include "AtlasData.h"

namespace akgr {

  struct HeroData {
    std::map<gf::Id, AtlasAnimation> animations;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, HeroData& data) {
    return ar | data.animations;
  }

}

#endif // AKGR_HERO_DATA_H
