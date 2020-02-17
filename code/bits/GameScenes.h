#ifndef GAME_SCENES_H
#define GAME_SCENES_H

#include <memory>

#include <gf/Gamepad.h>
#include <gf/Random.h>
#include <gf/ResourceManager.h>
#include <gf/SceneManager.h>

#include "Commands.h"
#include "Opening.h"
#include "Root.h"
#include "World.h"

#include "ui/Theme.h"

namespace akgr {

  struct OpeningAct;
  struct WorldAct;

  struct GameScenes : public gf::SceneManager {
    GameScenes(gf::Path searchDirectory);
    ~GameScenes();

    GameScenes(const GameScenes&) = delete;
    GameScenes(GameScenes&&) = delete;

    GameScenes& operator=(const GameScenes&) = delete;
    GameScenes& operator=(GameScenes&&) = delete;

    void startOpening();

    gf::ResourceManager resources;
    gf::Random random;
    gf::GamepadTracker tracker;

    ui::Theme theme;

    Commands commands;

    Root root;
    Opening opening;
    World world;

    std::unique_ptr<OpeningAct> openingAct;
    std::unique_ptr<WorldAct> worldAct;
  };

}

#endif // GAME_SCENES_H
