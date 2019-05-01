#ifndef AKGR_UI_SCENERY_H
#define AKGR_UI_SCENERY_H

#include <cstdint>

namespace akgr {
  namespace ui {

    struct WidgetIndexScenery {
      uint32_t choice;
      uint32_t count;

      void computeNextChoice() {
        choice = (choice + 1) % count;
      }

      void computePrevChoice() {
        choice = (choice + count - 1) % count;
      }
    };

  }
}

#endif // AKGR_UI_SCENERY_H
