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
#include <cstdlib>

#include <future>
#include <thread>

#include <boost/locale.hpp>

#include <gf/Action.h>
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/EntityContainer.h>
#include <gf/Event.h>
#include <gf/ModelContainer.h>
#include <gf/Paths.h>
#include <gf/Random.h>
#include <gf/RenderWindow.h>
#include <gf/ResourceManager.h>
#include <gf/VectorOps.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>

#include "bits/AreaRenderer.h"
#include "bits/AttributesRenderer.h"
#include "bits/CharacterRenderer.h"
#include "bits/Commands.h"
#include "bits/DialogRenderer.h"
#include "bits/Display.h"
#include "bits/HeroRenderer.h"
#include "bits/LogoRenderer.h"
#include "bits/MapRenderer.h"
#include "bits/MiniMapRenderer.h"
#include "bits/NotificationRenderer.h"
#include "bits/OpeningDriver.h"
#include "bits/OpeningScenery.h"
#include "bits/Script.h"
#include "bits/ShrineRenderer.h"
#include "bits/SlotSelectorRenderer.h"
#include "bits/StartMenuRenderer.h"
#include "bits/PhysicsRuntime.h"
#include "bits/UIData.h"
#include "bits/WorldData.h"
#include "bits/WorldDriver.h"
#include "bits/WorldProcessor.h"
#include "bits/WorldScenery.h"
#include "bits/WorldState.h"

#include "config.h"

namespace {

  enum class AdventureChoice {
    New,
    Saved,
  };

  constexpr const char *PreloadedTexture[] = {
    "tilesets/biomes.png",
    "tilesets/sprites-128.png"
  };

}

int main() {
  static constexpr gf::Vector2u ScreenSize(1024, 640);
  static constexpr gf::Vector2f ViewSize(800.0f, 800.0f);
  static constexpr gf::Vector2f ViewCenter = ViewSize / 2;

  boost::locale::generator localeGenerator;
  localeGenerator.add_messages_path(AKAGORIA_LOCALEDIR);
  localeGenerator.add_messages_domain("akagoria");
  std::locale::global(localeGenerator(""));

//   gf::Log::setLevel(gf::Log::Info);

  gf::Log::info("Locale is: %s\n", std::locale("").name().c_str());

  gf::Random random;

  gf::ResourceManager resources;
  resources.addSearchDir(AKAGORIA_DATADIR);

  // initialization

  gf::Window window("Akagoria, the revenge of Kalista", ScreenSize); //, ~gf::WindowHints::Resizable);
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(59);

  gf::RenderWindow renderer(window);

  akgr::Display display(resources);

  // views

  gf::ViewContainer views;

  gf::ExtendView mainView(ViewCenter, ViewSize);
  views.addView(mainView);

  gf::ScreenView hudView;
  views.addView(hudView);

  views.setInitialScreenSize(ScreenSize);

  gf::ZoomingViewAdaptor adaptor(renderer, mainView);

  // actions

  gf::ActionContainer windowActions;

  gf::Action closeWindowAction("Close window");
  closeWindowAction.addCloseControl();
  closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
  windowActions.addAction(closeWindowAction);

  gf::Action fullscreenAction("Fullscreen");
  fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
  windowActions.addAction(fullscreenAction);

  gf::Action debugPhysicsAction("Debug Physics");
  debugPhysicsAction.addScancodeKeyControl(gf::Scancode::F10);
  windowActions.addAction(debugPhysicsAction);

  gf::Action quickSaveAction("Quick Save");
  quickSaveAction.addScancodeKeyControl(gf::Scancode::F11);
  windowActions.addAction(quickSaveAction);

  gf::ActionContainer gameActions;
  akgr::Commands commands(gameActions);

  // preload textures

  for (auto path : PreloadedTexture) {
    resources.getTexture(path);
  }

  /*
   * opening
   */

  akgr::UIData uiData;

  if (!uiData.loadFromFile(resources.getAbsolutePath("ui.dat"))) {
    return EXIT_FAILURE;
  }

  akgr::OpeningScenery openingScenery;

  // entities

  gf::EntityContainer openingEntities;

  akgr::LogoRenderer logo(openingScenery, resources);
  openingEntities.addEntity(logo);

  akgr::StartMenuRenderer startMenu(uiData, openingScenery, display);
  openingEntities.addEntity(startMenu);

  akgr::OpeningSlotSelectorRenderer openingSlotLoader(openingScenery, display);
  openingEntities.addEntity(openingSlotLoader);

  // (empty) world

  akgr::WorldData worldData;
  akgr::WorldState worldState;
  akgr::WorldScenery worldScenery;
  akgr::Script script(resources, worldData, worldState);

  std::future<bool> worldLoading;
  auto worldLoader = [&](AdventureChoice choice) {
    gf::Log::info("Game loading...\n");
    gf::Clock loadingClock;

    if (!worldData.loadFromFile(resources.getAbsolutePath("akagoria.dat"))) {
      return false;
    }

    if (choice == AdventureChoice::Saved) {
      akgr::Slot& slot = openingScenery.selector.getSlot();
      assert(slot.active);
      gf::Log::debug("Loading from slot %i\n", openingScenery.selector.choice);
      worldState.loadFromFile(slot.path);
    }

    worldState.bind(worldData);
    worldScenery.bind(worldData, resources, random);
    script.initialize();

    if (choice == AdventureChoice::New) {
      script.start();
    }

//     std::this_thread::sleep_for(std::chrono::seconds(1)); // TODO: remove this in the future

    auto loadingTime = loadingClock.getElapsedTime();
    gf::Log::info("Game loaded in %d ms\n", loadingTime.asMilliseconds());


    return true;
  };

  // driver

  akgr::OpeningDriver openingDriver(openingScenery, commands);

  renderer.clear(gf::Color::White);

  gf::Clock clock;

  while (openingScenery.operation != akgr::OpeningOperation::Continue) {
    // 1. input

    gf::Event event;

    while (window.pollEvent(event)) {
      windowActions.processEvent(event);
      gameActions.processEvent(event);
      views.processEvent(event);
    }

    if (closeWindowAction.isActive()) {
      return EXIT_SUCCESS;
    }

    if (fullscreenAction.isActive()) {
      window.toggleFullscreen();
    }

    openingDriver.processCommands();

    switch (openingScenery.operation) {
      case akgr::OpeningOperation::Start:
        worldLoading = std::async(std::launch::async, worldLoader, AdventureChoice::New);
        openingScenery.operation = akgr::OpeningOperation::Wait;
        break;

      case akgr::OpeningOperation::Load:
        worldLoading = std::async(std::launch::async, worldLoader, AdventureChoice::Saved);
        openingScenery.operation = akgr::OpeningOperation::Wait;
        break;

      case akgr::OpeningOperation::Wait:
        if (worldLoading.valid() && worldLoading.wait_for(std::chrono::seconds::zero()) == std::future_status::ready) {

          if (!worldLoading.get()) {
            return EXIT_FAILURE;
          }

          openingScenery.operation = akgr::OpeningOperation::Continue;
        }
        break;

      case akgr::OpeningOperation::Quit:
        return EXIT_SUCCESS;

      default:
        // nothing to do
        break;
    }

    // 2. update

    auto delta = clock.restart();

    openingEntities.update(delta);

    // 3. draw

    renderer.clear();

    renderer.setView(hudView);
    openingEntities.render(renderer);

    renderer.display();

    windowActions.reset();
    gameActions.reset();
  }

  /*
   * game!
   */

  // entities

  gf::EntityContainer mainEntities;

  akgr::MapTileRenderer mapGround(akgr::Plane::Ground, worldData, worldState, worldScenery.map.groundTiles);
  mainEntities.addEntity(mapGround);

  akgr::MapTileRenderer mapLowTile(akgr::Plane::Low, worldData, worldState, worldScenery.map.lowTiles);
  mainEntities.addEntity(mapLowTile);

  akgr::MapSpriteRenderer mapLowSprite(akgr::Plane::Low, worldData, worldState, worldScenery.map.lowSprites);
  mainEntities.addEntity(mapLowSprite);

  akgr::MapTileRenderer mapHighTile(akgr::Plane::High, worldData, worldState, worldScenery.map.highTiles);
  mainEntities.addEntity(mapHighTile);

  akgr::MapSpriteRenderer mapHighSprite(akgr::Plane::High, worldData, worldState, worldScenery.map.highSprites);
  mainEntities.addEntity(mapHighSprite);

  akgr::HeroRenderer hero(worldState, resources);
  mainEntities.addEntity(hero);

  akgr::CharacterRenderer character(worldData, worldState);
  mainEntities.addEntity(character);

  akgr::ShrineRenderer shrine(worldScenery, worldState);
  mainEntities.addEntity(shrine);

  akgr::PhysicsDebugger debug(worldState.physics);
  mainEntities.addEntity(debug);


  gf::EntityContainer hudEntities;

  akgr::NotificationRenderer notifications(worldState, display);
  hudEntities.addEntity(notifications);

  akgr::DialogRenderer dialog(worldState, display);
  hudEntities.addEntity(dialog);

  akgr::AttributesRenderer attributes(worldState, resources);
  hudEntities.addEntity(attributes);

  akgr::WorldSlotSelectorRenderer worldSlotSaver(worldState, worldScenery, display);
  hudEntities.addEntity(worldSlotSaver);

  akgr::MiniMapRenderer miniMap(worldState);
  hudEntities.addEntity(miniMap);

  akgr::AreaRenderer area(resources, worldState, worldScenery);
  hudEntities.addEntity(area);

  // script

  akgr::PhysicsListener listener(script);
  worldState.physics.world.SetContactListener(&listener);

  akgr::PhysicsDraw draw(debug);
  bool debugPhysics = false;
//   worldState.physics.world.SetDebugDraw(&draw);

  // driver

  akgr::WorldProcessor worldProcessor(worldData, worldState, worldScenery, script);

  akgr::WorldDriver worldDriver(worldData, worldState, worldScenery, commands, script);

  // game loop

  renderer.clear(gf::Color::White);

  while (window.isOpen()) {
    // 1. input

    gf::Event event;

    while (window.pollEvent(event)) {
      windowActions.processEvent(event);
      gameActions.processEvent(event);
      views.processEvent(event);
      adaptor.processEvent(event);
    }

    if (closeWindowAction.isActive()) {
      window.close();
    }

    if (fullscreenAction.isActive()) {
      window.toggleFullscreen();
    }

    if (debugPhysicsAction.isActive()) {
      debugPhysics = !debugPhysics;
      worldState.physics.world.SetDebugDraw(debugPhysics ? &draw : nullptr);
    }

    if (quickSaveAction.isActive()) {
      akgr::Slot& slot = worldScenery.selector.quick;
      worldState.saveToFile(slot.path);
      slot.active = true;

      slot.meta.area = worldScenery.area.current->name;
      slot.save();

      gf::Log::debug("Quick save\n");
    }

    worldDriver.processCommands();

    // 2. update

    auto delta = clock.restart();

    worldProcessor.update(delta);

    mainEntities.update(delta);
    hudEntities.update(delta);

    mainView.setCenter(worldState.hero.physics.location.position);

    // 3. draw

    renderer.clear();

    renderer.setView(mainView);
    mainEntities.render(renderer);

    renderer.setView(hudView);
    hudEntities.render(renderer);

    renderer.display();

    windowActions.reset();
    gameActions.reset();
  }

  return EXIT_SUCCESS;
}
