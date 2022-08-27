#include "Widget.h"

#include <gf/VectorOps.h>

namespace akgr {
  namespace ui {

    Widget::Widget(Widget* parent)
    : m_parent(parent)
    {
    }

    Widget::~Widget() = default;

    Widget::Widget(Widget&& other) noexcept
    : m_parent(std::exchange(other.m_parent, nullptr))
    , m_position(other.m_position)
    , m_size(other.m_size)
    {

    }

    Widget& Widget::operator=(Widget&& other) noexcept {
      std::swap(m_parent, other.m_parent);
      std::swap(m_position, other.m_position);
      std::swap(m_size, other.m_size);
      return *this;
    }

    gf::Vector2f Widget::getAbsolutePosition() const noexcept {
      if (m_parent != nullptr) {
        return m_parent->getAbsolutePosition() + m_position;
      }

      return m_position;
    }

    void Widget::render([[maybe_unused]] gf::RenderTarget& target, [[maybe_unused]] const gf::RenderStates& states, [[maybe_unused]] Theme& theme) {
    }

    void Widget::doLayoutRequest() {
    }

    void Widget::doLayoutAllocation() {
    }

  }
}
