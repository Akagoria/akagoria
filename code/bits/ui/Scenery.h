#ifndef AKGR_UI_SCENERY_H
#define AKGR_UI_SCENERY_H

#include <cstddef>
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

    struct WidgetListScenery {
      uint32_t choice;
      uint32_t count;
      uint32_t itemsPerPage;

      void computeNextChoice() {
        choice = (choice + 1) % count;
      }

      void computePrevChoice() {
        choice = (choice + count - 1) % count;
      }

      void updateCount(uint32_t newCount) {
        count = newCount;

        if (count == 0) {
          choice = 0;
          return;
        }

        if (choice >= count) {
          choice = count - 1;
        }
      }
    };

    constexpr bool operator==(const WidgetListScenery& lhs, const WidgetListScenery& rhs) {
      return lhs.choice == rhs.choice && lhs.count == rhs.count && rhs.itemsPerPage == lhs.itemsPerPage;
    }

  }
}

#endif // AKGR_UI_SCENERY_H
