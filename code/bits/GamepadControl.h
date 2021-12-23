#ifndef AKGE_GAMEPAD_CONTROL_H
#define AKGE_GAMEPAD_CONTROL_H

#include <cstdint>

#include <gf/Control.h>

namespace akgr {

  class GamepadControl : public gf::Control {
  public:

    void processEvent(const gf::Event &event) override;

    float getAngle() const {
      return m_angle;
    }

  private:
    int16_t m_x = 0;
    int16_t m_y = 0;

    float m_angle = 0.0f;
  };

}

#endif // AKGE_GAMEPAD_CONTROL_H
