#ifndef GAME_SCENES_H
#define GAME_SCENES_H

#include <gf/Random.h>
#include <gf/ResourceManager.h>
#include <gf/SceneManager.h>

#include "Root.h"
#include "World.h"

namespace akgr {

  struct GameScenes : public gf::SceneManager {
    GameScenes(gf::Path searchDirectory);
    ~GameScenes() = default;

    GameScenes(const GameScenes&) = delete;
    GameScenes(GameScenes&&) = delete;

    GameScenes& operator=(const GameScenes&) = delete;
    GameScenes& operator=(GameScenes&&) = delete;

    gf::ResourceManager resources;
    gf::Random random;

    Root root;
    World world;
  };

}

#endif // GAME_SCENES_H
