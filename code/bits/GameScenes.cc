#include "GameScenes.h"

namespace akgr {

  namespace {
    constexpr gf::Vector2i InitialScreenSize(1024, 576);
  }

  GameScenes::GameScenes(gf::Path searchDirectory)
  : gf::SceneManager("Akagoria, the revenge of Kalista", InitialScreenSize, ~gf::WindowHints::Resizable)
  , resources({ searchDirectory })
  {

  }


}
