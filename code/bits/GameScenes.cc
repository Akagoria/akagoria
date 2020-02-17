#include "GameScenes.h"

#include <stdexcept>

#include "OpeningAct.h"
#include "WorldAct.h"

namespace akgr {

  namespace {
    constexpr gf::Vector2i InitialScreenSize(1024, 576);
  }

  GameScenes::GameScenes(gf::Path searchDirectory)
  : gf::SceneManager("Akagoria, the revenge of Kalista", InitialScreenSize, ~gf::WindowHints::Resizable)
  , resources({ searchDirectory })
  , theme(resources)
  , world(resources)
  {
    if (!root.data.loadFromFile(resources.getAbsolutePath("root.dat"))) {
      throw std::runtime_error("Could not load 'root.dat'.");
    }
  }

  GameScenes::~GameScenes() = default; // here because of unique_ptr's of incomplete type

  void GameScenes::startOpening() {
    openingAct = std::make_unique<OpeningAct>(*this);
    pushScene(openingAct->base);
    pushScene(openingAct->menu);
  }

}
