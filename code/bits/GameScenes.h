#ifndef GAME_SCENES_H
#define GAME_SCENES_H

#include <gf/SceneManager.h>
#include <gf/ResourceManager.h>

namespace akgr {

  struct GameScenes : public gf::SceneManager {
    GameScenes(gf::Path searchDirectory);
    ~GameScenes() = default;

    GameScenes(const GameScenes&) = delete;
    GameScenes(GameScenes&&) = delete;

    GameScenes& operator=(const GameScenes&) = delete;
    GameScenes& operator=(GameScenes&&) = delete;

    gf::ResourceManager resources;

  };

}

#endif // GAME_SCENES_H
