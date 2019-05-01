#include "GameMenuScenery.h"

#include <cassert>

namespace akgr {

  GameMenuScenery::Choice GameMenuScenery::getChoice() const {
    switch (index.choice) {
      case 0:
        return Choice::Inventory;
      case 1:
        return Choice::Quests;
      case 2:
        return Choice::Skills;
      case 3:
        return Choice::Options;
      case 4:
        return Choice::BackToAdventure;
      case 5:
        return Choice::BackToRealLife;
      default:
        assert(false);
        return Choice::BackToRealLife;
    }
  }

}
