#include "Widgets.h"

#include <gf/Coordinates.h>
#include <gf/Curves.h>
#include <gf/Log.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/StringUtils.h>
#include <gf/Text.h>

namespace akgr {
  namespace ui {

    namespace {

      constexpr gf::Color4f RpgBlue(0.015625, 0.03125, 0.515625, 0.9f);
      constexpr float Padding = 0.01f;
      constexpr float Thickness = 0.002f;

      constexpr float ArrowSize = 64.0f;

    } // namespace

    /*
     * ContainerWidget
     */

    ContainerWidget::ContainerWidget(Widget* parent)
    : Widget(parent)
    {

    }

    ContainerWidget::~ContainerWidget() {
      for (auto child : m_children) {
        delete child;
      }
    }

    void ContainerWidget::render(gf::RenderTarget& target, const gf::RenderStates& states, Theme& theme) {
      for (auto child : m_children) {
        child->render(target, states, theme);
      }
    }

    /*
     * BoxWidget
     */

    BoxWidget::BoxWidget(Widget* parent, Aspect aspect)
    : ContainerWidget(parent)
    , m_aspect(aspect)
    , m_positioning(Positioning::Middle)
    , m_margin({ 0.0f, 0.0f })
    , m_offset({ 0.0f, 0.0f })
    , m_spacing(0.0f)
    {

    }

    void BoxWidget::render(gf::RenderTarget& target, const gf::RenderStates& states, Theme& theme) {
      for (auto child : *this) {
        child->render(target, states, theme);
      }
    }

    void BoxWidget::doLayoutRequest() {
      gf::Vector2f container = 2 * m_margin;

      float gf::Vector2f::* axis0 = m_aspect == Aspect::Horizontal ? &gf::Vector2f::width : &gf::Vector2f::height;
      float gf::Vector2f::* axis1 = m_aspect == Aspect::Vertical   ? &gf::Vector2f::width : &gf::Vector2f::height;

      for (auto child : *this) {
        child->requestLayout();
        gf::Vector2f size = child->getSize();
        container.*axis0 += size.*axis0;
        container.*axis1 = std::max(container.*axis1, size.*axis1 + 2 * m_margin.*axis1);
      }

      if (getChildrenCount() > 1) {
        container.*axis0 += (getChildrenCount() - 1) * m_spacing;
      }

      container += m_offset;
      setSize(gf::max(container, getSize()));
    }

    void BoxWidget::doLayoutAllocation() {
      gf::Vector2f container = getSize();

      float gf::Vector2f::* axis0 = m_aspect == Aspect::Horizontal ? &gf::Vector2f::width : &gf::Vector2f::height;
      float gf::Vector2f::* axis1 = m_aspect == Aspect::Vertical   ? &gf::Vector2f::width : &gf::Vector2f::height;

      gf::Vector2f position = m_margin;

      for (auto child : *this) {
        gf::Vector2f size = child->getSize();

        switch (m_positioning) {
          case Positioning::Minimum:
            position.*axis1 = m_margin.*axis1;
            break;
          case Positioning::Middle:
            position.*axis1 = (container.*axis1 - size.*axis1) / 2;
            break;
          case Positioning::Maximum:
            position.*axis1 = container.*axis1 - size.*axis1 - m_margin.*axis1;
            break;
          case Positioning::Fill:
            position.*axis1 = m_margin.*axis1;
            size.*axis1 = container.*axis1 - 2 * m_margin.*axis1;
            break;
        }

        child->setPosition(m_offset + position);
        child->setSize(size);
        child->allocateLayout();

        position.*axis0 += size.*axis0;
        position.*axis0 += m_spacing;
      }
    }

    /*
     * StackWidget
     */

    StackWidget::StackWidget(Widget* parent)
    : ContainerWidget(parent)
    , m_positioning(Positioning::Middle)
    , m_margin({ 0.0f, 0.0f })
    {

    }

    void StackWidget::doLayoutRequest() {
      gf::Vector2f container = getSize();

      for (auto child : *this) {
        child->requestLayout();
        container = gf::max(container, child->getSize() + 2 * m_margin);
      }

      setSize(container);
    }

    void StackWidget::doLayoutAllocation() {
      gf::Vector2f container = getSize();

      for (auto child : *this) {
        gf::Vector2f size = child->getSize();
        gf::Vector2f position = m_margin;

        switch (m_positioning) {
          case Positioning::Minimum:
            position = m_margin;
            break;
          case Positioning::Middle:
            position = (container - size) / 2;
            break;
          case Positioning::Maximum:
            position = container - size - m_margin;
            break;
          case Positioning::Fill:
            size = container - 2 * m_margin;
            break;
        }

        child->setPosition(position);
        child->setSize(size);
        child->allocateLayout();
      }
    }

    /*
     * ListWidget
     */

    ListWidget::ListWidget(Widget* parent, ListModel& model)
    : Widget(parent)
    , m_model(model)
    , m_positioning(Positioning::Middle)
    , m_margin({ 0.0f, 0.0f })
    , m_offset({ 0.0f, 0.0f })
    , m_spacing(0.0f)
    {

    }

    void ListWidget::doLayoutRequest() {
      gf::Vector2f container = 2 * m_margin;

      m_model.update(this);
      std::size_t count = m_model.getWidgetCount();

      for (std::size_t i = 0; i < count; ++i) {
        auto widget = m_model.getWidget(i);
        widget->requestLayout();
        gf::Vector2f size = widget->getSize();
        container.height += size.height;
        container.width = std::max(container.width, size.width + 2 * m_margin.width);
      }

      if (count > 0) {
        container.height += (count - 1) * m_spacing;
      }

      setSize(m_offset + container);
    }

    void ListWidget::doLayoutAllocation() {
      gf::Vector2f container = getSize();

      gf::Vector2f position = m_margin;

      std::size_t count = m_model.getWidgetCount();

      for (std::size_t i = 0; i < count; ++i) {
        auto widget = m_model.getWidget(i);
        gf::Vector2f size = widget->getSize();

        switch (m_positioning) {
          case Positioning::Minimum:
            position.x = m_margin.width;
            break;
          case Positioning::Middle:
            position.x = (container.width - size.width) / 2;
            break;
          case Positioning::Maximum:
            position.x = container.width - size.width - m_margin.width;
            break;
          case Positioning::Fill:
            position.x = m_margin.width;
            size.width = container.width - 2 * m_margin.width;
            break;
        }

        widget->setPosition(m_offset + position);
        widget->setSize(size);
        widget->allocateLayout();

        position.y += size.height;
        position.y += m_spacing;
      }
    }

    /*
     * FrameWidget
     */

    FrameWidget::FrameWidget(Widget* parent)
    : BoxWidget(parent, Aspect::Vertical)
    {
    }

    void FrameWidget::render(gf::RenderTarget& target, const gf::RenderStates& states, Theme& theme) {
      gf::Coordinates coords(target);

      gf::Vector2f position = coords.getRelativePoint(getAbsolutePosition());
      gf::Vector2f size = coords.getRelativeSize(getSize());
      float thickness = coords.getRelativeSize({ 0.0f, Thickness }).height;

      gf::RoundedRectangleShape shape(size, 5.0f);
      shape.setPosition(position);
      shape.setColor(RpgBlue);
      shape.setOutlineColor(gf::Color::White);
      shape.setOutlineThickness(thickness);
      target.draw(shape, states);

      BoxWidget::render(target, states, theme);
    }

    /*
     * MenuWidget
     */

    MenuWidget::MenuWidget(Widget* parent, const WidgetIndexScenery& scenery)
    : BoxWidget(parent, Aspect::Vertical)
    , m_scenery(scenery)
    {
      setPositioning(Positioning::Minimum);
      setOffset(gf::dirx(0.02f));
      setSpacing(0.01f);
      setMargin({ 0.01f, 0.01f });
    }

    void MenuWidget::render(gf::RenderTarget& target, const gf::RenderStates& states, Theme& theme) {
      BoxWidget::render(target, states, theme);

      std::size_t i = m_scenery.choice;
      assert(i < getChildrenCount());
      auto widget = getChild(i);
      assert(widget);
      auto widgetPosition = widget->getAbsolutePosition();
      auto widgetSize = widget->getSize();

      gf::Vector2f position = widgetPosition + gf::projy(widgetSize) / 2 - gf::dirx(0.015f);

      gf::Coordinates coords(target);

      float height = coords.getRelativeSize({ 0.0f, 0.03f }).height;

      gf::Sprite sprite(theme.getArrowTexture(), gf::RectF::fromPositionSize({ 0.25f, 0.0f }, { 0.125f, 0.125f }));
      sprite.setPosition(coords.getRelativePoint(position));
      sprite.setScale(height / ArrowSize);
      sprite.setAnchor(gf::Anchor::Center);
      target.draw(sprite, states);
    }

    /*
     * ChoiceWidget
     */

    ChoiceWidget::ChoiceWidget(Widget* parent, const WidgetIndexScenery& scenery)
    : StackWidget(parent)
    , m_scenery(scenery)
    {

    }

    void ChoiceWidget::render(gf::RenderTarget& target, const gf::RenderStates& states, Theme& theme) {
      std::size_t i = m_scenery.choice;
      assert(i < getChildrenCount());
      auto widget = getChild(i);
      assert(widget);
      widget->render(target, states, theme);

      gf::Coordinates coords(target);

      gf::Vector2f position = coords.getRelativePoint(getAbsolutePosition());
      gf::Vector2f size = coords.getRelativeSize(getSize());

      float square = size.height;

      gf::CompoundCurve back({ square / 2, square / 4 });
      back.lineTo({ square / 4, square / 2 });
      back.lineTo({ square / 2, 3 * square / 4 });
      back.setPosition(position);
      back.setType(gf::Curve::Outlined);
      back.setWidth(size.height / 10);
      back.setColor(gf::Color::White);
      back.setOutlineColor(gf::Color::Black);
      back.setOutlineThickness(1);
      target.draw(back, states);

      gf::CompoundCurve forth({ - square / 2, square / 4 });
      forth.lineTo({ - square / 4, square / 2 });
      forth.lineTo({ - square / 2, 3 * square / 4 });
      forth.setPosition({ position.x + size.width, position.y });
      forth.setType(gf::Curve::Outlined);
      forth.setWidth(size.height / 10);
      forth.setColor(gf::Color::White);
      forth.setOutlineColor(gf::Color::Black);
      forth.setOutlineThickness(1);
      target.draw(forth, states);
    }

    /*
     * CatalogueWidget
     */

    CatalogueWidget::CatalogueWidget(Widget* parent, ListModel& model, const WidgetListScenery& scenery)
    : ListWidget(parent, model)
    , m_scenery(scenery)
    {
      setPositioning(Positioning::Minimum);
      setOffset(gf::dirx(0.02f));
      setSpacing(0.01f);
      setMargin({ 0.01f, 0.01f });
    }

    void CatalogueWidget::render(gf::RenderTarget& target, const gf::RenderStates& states, Theme& theme) {
      std::size_t count = getModel().getWidgetCount();

      if (count > m_scenery.length) {
        gf::Log::debug("count: %zu, m_scenery.length: %zu\n", count, m_scenery.length);
      }

      assert(count <= m_scenery.length);

      if (count == 0) {
        return;
      }

      for (std::size_t i = 0; i < count; ++i) {
        getModel().getWidget(i)->render(target, states, theme);
      }

      std::size_t i = m_scenery.choice - m_scenery.start;
      assert(i < count);
      auto widget = getModel().getWidget(i);
      assert(widget);
      auto widgetPosition = widget->getAbsolutePosition();
      auto widgetSize = widget->getSize();

      gf::Vector2f position = widgetPosition + gf::projy(widgetSize) / 2 - gf::dirx(0.015f);

      gf::Coordinates coords(target);

      float height = coords.getRelativeSize({ 0.0f, 0.03f }).height;

      gf::Sprite sprite(theme.getArrowTexture(), gf::RectF::fromPositionSize({ 0.25f, 0.0f }, { 0.125f, 0.125f }));
      sprite.setPosition(coords.getRelativePoint(position));
      sprite.setScale(height / ArrowSize);
      sprite.setAnchor(gf::Anchor::Center);
      target.draw(sprite, states);
    }

    /*
     * LabelWidget
     */

    LabelWidget::LabelWidget(Widget* parent, std::string caption, float characterSize)
    : Widget(parent)
    , m_caption(std::move(caption))
    , m_characterSize(characterSize)
    {

    }

    void LabelWidget::render(gf::RenderTarget& target, const gf::RenderStates& states, Theme& theme) {
      gf::Coordinates coords(target);

      gf::Vector2f position = coords.getRelativePoint(getAbsolutePosition());
      gf::Vector2f size = coords.getRelativeSize(getSize());
      unsigned characterSize = coords.getRelativeCharacterSize(m_characterSize);

      gf::Text text(m_caption, theme.getFont(), characterSize);
      text.setColor(gf::Color::White);
      text.setOutlineColor(gf::Color::Black);
      text.setOutlineThickness(1);
      text.setAnchor(gf::Anchor::CenterLeft);

      gf::Vector2f bounds = text.getLocalBounds().getSize();
      text.setPosition(position + gf::diry(size.height / 2));
      target.draw(text, states);

      if (bounds.width > size.width || bounds.height > size.height) {
        gf::Log::debug("LabelWidget::render(): text can not fit in area, area is (%g, %g), bounds is (%g, %g), text is: \"%s\"\n", size.width, size.height, bounds.width, bounds.height, gf::escapeString(m_caption).c_str());
      }
    }

    /*
     * TextWidget
     */

    TextWidget::TextWidget(Widget* parent, std::string caption, float characterSize, gf::Alignment alignment)
    : Widget(parent)
    , m_caption(std::move(caption))
    , m_characterSize(characterSize)
    , m_alignment(alignment)
    {

    }

    void TextWidget::render(gf::RenderTarget& target, const gf::RenderStates& states, Theme& theme) {
      gf::Coordinates coords(target);

      gf::Vector2f position = coords.getRelativePoint(getAbsolutePosition());
      gf::Vector2f size = coords.getRelativeSize(getSize());
      unsigned characterSize = coords.getRelativeCharacterSize(m_characterSize);

      gf::Text text(m_caption, theme.getFont(), characterSize);
      text.setParagraphWidth(size.width);
      text.setAlignment(m_alignment);
      text.setColor(gf::Color::White);
      text.setOutlineColor(gf::Color::Black);
      text.setOutlineThickness(1);
      text.setAnchor(gf::Anchor::TopLeft);

      gf::Vector2f bounds = text.getLocalBounds().getSize();
      text.setPosition(position);
      target.draw(text, states);

      if (bounds.width > size.width || bounds.height > size.height) {
        gf::Log::debug("TextWidget::render(): text can not fit in area, area is (%g, %g), bounds is (%g, %g), text is: \"%s\"\n", size.width, size.height, bounds.width, bounds.height, gf::escapeString(m_caption).c_str());
      }
    }

    /*
     * DoubleTextWidget
     */

    DoubleTextWidget::DoubleTextWidget(Widget* parent, std::string leftCaption, std::string rightCaption, float characterSize)
    : Widget(parent)
    , m_leftCaption(leftCaption)
    , m_rightCaption(rightCaption)
    , m_characterSize(characterSize)
    {

    }

    void DoubleTextWidget::render(gf::RenderTarget& target, const gf::RenderStates& states, Theme& theme) {
      gf::Coordinates coords(target);

      gf::Vector2f position = coords.getRelativePoint(getAbsolutePosition());
      gf::Vector2f size = coords.getRelativeSize(getSize());
      unsigned characterSize = coords.getRelativeCharacterSize(m_characterSize);

      auto displayText = [&](const std::string& caption, gf::Alignment alignment) {
        gf::Text text(caption, theme.getFont(), characterSize);
        text.setParagraphWidth(size.width);
        text.setAlignment(alignment);
        text.setColor(gf::Color::White);
        text.setOutlineColor(gf::Color::Black);
        text.setOutlineThickness(1);
        text.setAnchor(gf::Anchor::CenterLeft);

        gf::Vector2f bounds = text.getLocalBounds().getSize();
        text.setPosition(position + gf::diry(size.height / 2));
        target.draw(text, states);

        if (bounds.width > size.width || bounds.height > size.height) {
          gf::Log::debug("DoubleTextWidget::render(): text can not fit in area, area is (%g, %g), bounds is (%g, %g), text is: \"%s\"\n", size.width, size.height, bounds.width, bounds.height, gf::escapeString(caption).c_str());
        }
      };

      displayText(m_rightCaption, gf::Alignment::Right);
      displayText(m_leftCaption, gf::Alignment::Left);
    }

  }
}
