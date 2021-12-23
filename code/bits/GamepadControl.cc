#include "GamepadControl.h"

#include <limits>

#include <gf/Event.h>
#include <gf/Math.h>
#include <gf/VectorOps.h>

namespace akgr {

  void GamepadControl::processEvent(const gf::Event &event) {
    if (event.type != gf::EventType::GamepadAxisMoved) {
      return;
    }

    switch (event.gamepadAxis.axis) {
      case gf::GamepadAxis::LeftX:
        m_x = event.gamepadAxis.value;
        break;
      case gf::GamepadAxis::LeftY:
        m_y = event.gamepadAxis.value;
        break;
      default:
        return;
    }

    float x = static_cast<float>(m_x) / std::numeric_limits<int16_t>::max();
    float y = static_cast<float>(m_y) / std::numeric_limits<int16_t>::max();

    if (gf::squareLength(gf::vec(x, y)) > gf::square(0.5f)) {
      m_angle = std::atan2(y, x);
      setActive(true);
    } else {
      setActive(false);
    }

  }

}
