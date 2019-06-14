#ifndef AKGR_UI_MODELS_H
#define AKGR_UI_MODELS_H

#include <cstddef>

namespace akgr {
  namespace ui {
    class Widget;

    class ListModel {
    public:
      virtual ~ListModel();

      virtual void update(Widget* parent) = 0;
      virtual std::size_t getWidgetCount() = 0;
      virtual Widget* getWidget(std::size_t i) = 0;

    };

  }
}


#endif // AKGR_UI_MODELS_H
