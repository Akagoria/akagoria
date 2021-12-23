/*
 * Akagoria, the revenge of Kalista
 * a single-player RPG in an open world with a top-down view.
 *
 * Copyright (c) 2013-2018, Julien Bernard
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "SlotSelectorRenderer.h"

#include <ctime>
#include <array>
#include <string>

#include <boost/locale.hpp>

#include <gf/Id.h>

#include "ui/Common.h"

using namespace gf::literals;

namespace akgr {

  namespace {

    std::string getSlotInfo(const Slot& slot, int index) {
      if (!slot.active) {
        return "Slot #" + std::to_string(index + 1) + "\n---\n---";
      }

      std::string str;

      str += "Slot #" + std::to_string(index + 1) + '\n';
      str += boost::locale::gettext(slot.meta.area.c_str()) + '\n';

      std::time_t time = std::chrono::duration_cast<std::chrono::seconds>(slot.time.time_since_epoch()).count();

//       auto time = std::chrono::system_clock::to_time_t(std::chrono::time_point_cast<std::chrono::system_clock::duration>(slot.time)); // TODO: need std::chrono::time_point_cast?

      static constexpr std::size_t TimeInfoSize = 1024;

      std::array<char, TimeInfoSize> timeInfo;
      std::strftime(timeInfo.data(), timeInfo.size(), "%F %T", std::localtime(&time));
      str += timeInfo.data();

      return str;
    }

    class SlotWidget : public ui::TextWidget {
    public:
      SlotWidget(ui::Widget* parent, const Slot& slot, int index)
      : ui::TextWidget(parent, getSlotInfo(slot, index))
      , m_slot(slot)
      , m_index(index)
      , m_last(slot.time)
      {
        setSize({ 0.30f, ui::Common::DefaultCharacterSize * 1.2 * 3.0f });
      }

      virtual void render(gf::RenderTarget& target, const gf::RenderStates& states, ui::Theme& theme) override {
        if (m_slot.time > m_last) {
          setCaption(getSlotInfo(m_slot, m_index));
          m_last = m_slot.time;
        }

        TextWidget::render(target, states, theme);
      }

    private:
      const Slot& m_slot;
      int m_index;
      std::filesystem::file_time_type m_last;
    };

  }

  SlotSelectorRenderer::SlotSelectorRenderer(const RootData& data, const RootScenery& scenery, ui::Theme& theme)
  : m_data(data)
  , m_scenery(scenery)
  , m_theme(theme)
  , m_frame(nullptr)
  {
    auto menu = m_frame.add<ui::MenuWidget>(m_scenery.selector.index);

    for (int i = 0; i < SlotSelectorScenery::SlotCount; ++i) {
      auto innerFrame = menu->add<ui::FrameWidget>();
      innerFrame->setPositioning(ui::Positioning::Minimum);
      innerFrame->setMargin({ 0.01f, 0.01f });

      innerFrame->add<SlotWidget>(m_scenery.selector.games[i], i);
    }

    auto label = menu->add<ui::LabelWidget>(m_data.getUIMessage("MenuBack"_id));
    label->setSize(ui::Common::DefaultCaptionSize);

    m_frame.setPosition(ui::Common::Position);
    m_frame.computeLayout();
  }

  void SlotSelectorRenderer::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    m_frame.render(target, states, m_theme);
  }

}
