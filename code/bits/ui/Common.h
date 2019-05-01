#ifndef AKGR_UI_COMMON_H
#define AKGR_UI_COMMON_H

#include <gf/Vector.h>

namespace akgr {
  namespace ui {

    struct Common {

      static constexpr gf::Vector2f Position = { 0.002f, 0.002f };

      static constexpr float DefaultCharacterSize = 0.03f;

      static constexpr gf::Vector2f DefaultCaptionSize = { 0.3f, DefaultCharacterSize * 1.3f };

    };

  }
}


#endif // AKGR_UI_COMMON_H
