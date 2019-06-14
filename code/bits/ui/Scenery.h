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

    struct WidgetListScenery {
      std::size_t choice;

      std::size_t start;
      std::size_t length;

      void computeNextChoice(std::size_t count) {
        updateCount(count);

        if (choice == count - 1) {
          return;
        }

        ++choice;

        if (start + length < count && choice >= start + length - 1) {
          ++start;
        }
      }

      void computePrevChoice(std::size_t count) {
        updateCount(count);

        if (choice == 0) {
          return;
        }

        --choice;

        if (start > 0 && choice < start + 1) {
          -- start;
        }
      }

      void updateCount(std::size_t count) {
        if (count > 0) {
          if (choice >= count) {
            choice = count - 1;
          }

          if (start >= count) {
            start = count - 1;
          }
        } else {
          choice = 0;
          start = 0;
        }
      }
    };

    constexpr bool operator==(const WidgetListScenery& lhs, const WidgetListScenery& rhs) {
      return lhs.choice == rhs.choice && lhs.start == rhs.start && rhs.length == lhs.length;
    }

  }
}

#endif // AKGR_UI_SCENERY_H
