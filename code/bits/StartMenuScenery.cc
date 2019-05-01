#include "StartMenuScenery.h"

#include <cassert>

namespace akgr {

  StartMenuScenery::Choice StartMenuScenery::getChoice() const {
    switch (index.choice) {
      case 0:
        return Choice::StartAdventure;
      case 1:
        return Choice::LoadAdventure;
      case 2:
        return Choice::Options;
      case 3:
        return Choice::Quit;
      default:
        assert(false);
        return Choice::Quit;
    }
  }

}

