#ifndef AKGR_UI_WIDGET_H
#define AKGR_UI_WIDGET_H

#include <memory>
#include <vector>

#include <gf/Vector.h>
#include <gf/RenderStates.h>
#include <gf/RenderTarget.h>

#include "Theme.h"

namespace akgr {
  namespace ui {

    class Widget {
    public:
      Widget(Widget* parent);
      virtual ~Widget();

      Widget(const Widget&) = delete;
      Widget& operator=(const Widget&) = delete;

      Widget(Widget&& other) noexcept;
      Widget& operator=(Widget&& other) noexcept;

      Widget* getParent() noexcept { return m_parent; }
      const Widget* getParent() const noexcept { return m_parent; }
      void setParent(Widget* parent) noexcept { m_parent = parent; }

      gf::Vector2f getPosition() const noexcept { return m_position; }
      void setPosition(gf::Vector2f position) noexcept { m_position = position; }

      gf::Vector2f getAbsolutePosition() const noexcept;

      gf::Vector2f getSize() const noexcept { return m_size; }
      void setSize(gf::Vector2f size) noexcept { m_size = size; }

      virtual void render(gf::RenderTarget& target, const gf::RenderStates& states, Theme& theme);

      void requestLayout() { doLayoutRequest(); }
      void allocateLayout() { doLayoutAllocation(); }

      void computeLayout() {
        requestLayout();
        allocateLayout();
      }

    private:
      virtual void doLayoutRequest();
      virtual void doLayoutAllocation();

    private:
      Widget* m_parent = nullptr;
      gf::Vector2f m_position = { 0.0f, 0.0f };
      gf::Vector2f m_size = { 0.0f, 0.0f };
    };

  }
}

#endif // AKGR_UI_WIDGET_H
