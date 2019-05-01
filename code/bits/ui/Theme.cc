#include "Theme.h"

namespace akgr {
  namespace ui {

    Theme::Theme(gf::ResourceManager& resources)
    : m_font(resources.getFont("fonts/DejaVuSans.ttf")) // fonts/Philosopher-Regular.ttf
    , m_arrow(resources.getTexture("pictures/icons.png"))
    {
      m_arrow.setSmooth();
    }

  }
}


