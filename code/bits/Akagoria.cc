#include "Akagoria.h"

#include <stdexcept>
#include <thread>

#include <gf/Clock.h>
#include <gf/Id.h>
#include <gf/Log.h>
#include <gf/SharedGraphics.h>

#include "OpeningAct.h"
#include "WorldAct.h"

using namespace gf::literals;

namespace akgr {

  namespace {
    constexpr gf::Vector2i InitialScreenSize(1280, 720);

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
  , world(resources, *this)
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

    slots.loadSlotMeta();
  }

  Akagoria::~Akagoria() = default; // here because of unique_ptr's of incomplete type

  void Akagoria::startOpening() {
    openingAct = std::make_unique<OpeningAct>(*this);
    gf::Ref<gf::Scene> scenes[] = { openingAct->base, openingAct->menu };
    pushScenes(scenes);
  }

  bool Akagoria::loadWorld(AdventureChoice choice) {
    gf::SharedGraphics shared(getWindow());

    gf::Log::info("Game loading...\n");
    gf::Clock loadingClock;

    if (!world.data.loadFromFile(resources.getAbsolutePath("akagoria.dat"))) {
      return false;
    }

    auto loadingDataTime = loadingClock.getElapsedTime();
    gf::Log::info("Data loaded in %d ms\n", loadingDataTime.asMilliseconds());

    if (choice == AdventureChoice::Saved) {
      assert(root.scenery.selector.index.choice < SlotManager::SlotCount);
      Slot& slot = slots.devices[root.scenery.selector.index.choice];
      assert(slot.active);
      gf::Log::debug("Loading from slot %i\n", root.scenery.selector.index.choice);
      world.state.loadFromFile(slot.path);
    } else {
      // TODO: remove this later
      world.state.hero.weapon.ref.id = "SmallSword"_id;
    }

    world.state.bind(world.data);
    world.scenery.bind(world.state);
    world.script.bind();
    world.script.initialize();

    if (choice == AdventureChoice::New) {
      world.script.start();
    }

    worldAct = std::make_unique<WorldAct>(*this);

    // preload atlases

    for (auto & atlas : world.data.atlases) {
      resources.getTexture(atlas.path);
    }

    auto loadingTime = loadingClock.getElapsedTime();
    gf::Log::info("Game loaded in %d ms\n", loadingTime.asMilliseconds());

    return true;
  }

  void Akagoria::startWorld() {
    gf::Ref<gf::Scene> scenes[] = { worldAct->base, worldAct->travel };
    replaceAllScenes(scenes); // TODO: put a segue?
  }

  void Akagoria::doGlobalProcessEvent(const gf::Event& event) {
    tracker.processEvent(event);
  }

}
