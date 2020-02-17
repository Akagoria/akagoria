#include "Akagoria.h"

#include <stdexcept>
#include <thread>

#include <gf/Clock.h>
#include <gf/Id.h>
#include <gf/Log.h>

#include "OpeningAct.h"
#include "WorldAct.h"

using namespace gf::literals;

namespace akgr {

  namespace {
    constexpr gf::Vector2i InitialScreenSize(1024, 576);

    gf::Vector2i computeScreenSize(OptionsData::Display display) {
      switch (display) {
        case OptionsData::Display::Window_960x540:
          return { 960, 540 };
        case OptionsData::Display::Window_1024x576:
          return { 1024, 576 };
        case OptionsData::Display::Window_1152x648:
          return { 1152, 648 };
        case OptionsData::Display::Window_1280x720:
          return { 1280, 720 };
        default:
          break;
      }

      return InitialScreenSize;
    }

  }

  Akagoria::Akagoria(gf::Path searchDirectory)
  : gf::SceneManager("Akagoria, the revenge of Kalista", InitialScreenSize, ~gf::WindowHints::Resizable)
  , resources({ searchDirectory })
  , theme(resources)
  , world(resources)
  {
    if (!root.data.loadFromFile(resources.getAbsolutePath("root.dat"))) {
      throw std::runtime_error("Could not load 'root.dat'.");
    }

    auto display = root.scenery.options.data.getDisplay();

    if (display == OptionsData::Display::Fullscreen) {
      getWindow().setFullscreen();
    } else {
      getWindow().setSize(computeScreenSize(display));
    }

  }

  Akagoria::~Akagoria() = default; // here because of unique_ptr's of incomplete type

  void Akagoria::startOpening() {
    openingAct = std::make_unique<OpeningAct>(*this);
    pushScene(openingAct->base);
    pushScene(openingAct->menu);
  }

  bool Akagoria::loadWorld(AdventureChoice choice) {
    gf::Log::info("Game loading...\n");
    gf::Clock loadingClock;

    if (!world.data.loadFromFile(resources.getAbsolutePath("akagoria.dat"))) {
      return false;
    }

    if (choice == AdventureChoice::Saved) {
      Slot& slot = root.scenery.selector.getSlot();
      assert(slot.active);
      gf::Log::debug("Loading from slot %i\n", root.scenery.selector.index.choice);
      world.state.loadFromFile(slot.path);
    } else {
      // TODO: remove this later
      world.state.hero.weapon.ref.id = "SmallSword"_id;
    }

    world.state.bind(world.data);
    world.scenery.bind(world.data, resources, random);
    world.script.bind();
    world.script.initialize();

    if (choice == AdventureChoice::New) {
      world.script.start();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // TODO: remove this in the future

    auto loadingTime = loadingClock.getElapsedTime();
    gf::Log::info("Game loaded in %d ms\n", loadingTime.asMilliseconds());

    return true;
  }

}
