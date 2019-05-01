#ifndef AKGR_UI_THEME_H
#define AKGR_UI_THEME_H

#include <gf/ResourceManager.h>

namespace akgr {
  namespace ui {

    class Theme {
    public:
      Theme(gf::ResourceManager& resources);

      gf::Font& getFont() {
        return m_font;
      }

      gf::Texture& getArrowTexture() {
        return m_arrow;
      }

    private:
      gf::Font& m_font;
      gf::Texture& m_arrow;
    };

  }
}


#endif // AKGR_UI_THEME_H
