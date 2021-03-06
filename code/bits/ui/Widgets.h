#ifndef AKGR_UI_WIDGETS_H
#define AKGR_UI_WIDGETS_H

#include <string>

#include <gf/Alignment.h>

#include "Models.h"
#include "Scenery.h"
#include "Widget.h"

namespace akgr {
  namespace ui {

    enum class Aspect {
      Horizontal,
      Vertical,
    };

    enum class Positioning {
      Minimum,
      Middle,
      Maximum,
      Fill,
    };

    /*
     * Layout widgets
     */

    class ContainerWidget : public Widget {
    public:
      ContainerWidget(Widget* parent);
      ~ContainerWidget();

      std::size_t getChildrenCount() const noexcept { return m_children.size(); }

      Widget* getChild(std::size_t i) noexcept { return m_children[i]; }
      const Widget* getChild(std::size_t i) const noexcept { return m_children[i]; }

      Widget* operator[](std::size_t i) noexcept { return m_children[i]; }
      const Widget* operator[](std::size_t i) const noexcept { return m_children[i]; }

      void addChild(std::unique_ptr<Widget> widget) noexcept { m_children.push_back(widget.release()); }

      template<typename Class, typename... Args>
      Class* add(Args&& ... args) {
        auto child = new Class(this, std::forward<Args>(args)...);
        m_children.push_back(child);
        return child;
      }

      Widget** begin() noexcept { return m_children.data(); }
      Widget** end() noexcept { return m_children.data() + m_children.size(); }

      virtual void render(gf::RenderTarget& target, const gf::RenderStates& states, Theme& theme) override;

    private:
      std::vector<Widget*> m_children;
    };

    class BoxWidget : public ContainerWidget {
    public:
      BoxWidget(Widget* parent, Aspect aspect);

      void setAspect(Aspect aspect) noexcept { m_aspect = aspect; }
      Aspect getAspect() const noexcept { return m_aspect; }

      void setPositioning(Positioning positioning) noexcept { m_positioning = positioning; }
      Positioning getPositioning() const noexcept { return m_positioning; }

      void setMargin(gf::Vector2f margin) noexcept { m_margin = margin; }
      gf::Vector2f getMargin() const noexcept { return m_margin; }

      void setOffset(gf::Vector2f offset) noexcept { m_offset = offset; }
      gf::Vector2f getOffset() const noexcept { return m_offset; }

      void setSpacing(float spacing) noexcept { m_spacing = spacing; }
      float getSpacing() const noexcept { return m_spacing; }

      virtual void render(gf::RenderTarget& target, const gf::RenderStates& states, Theme& theme) override;

    private:
      virtual void doLayoutRequest() override;
      virtual void doLayoutAllocation() override;

    private:
      Aspect m_aspect;
      Positioning m_positioning;
      gf::Vector2f m_margin;
      gf::Vector2f m_offset;
      float m_spacing;
    };

    class StackWidget : public ContainerWidget {
    public:
      StackWidget(Widget* parent);

      void setPositioning(Positioning positioning) noexcept { m_positioning = positioning; }
      Positioning getPositioning() const noexcept { return m_positioning; }

      void setMargin(gf::Vector2f margin) noexcept { m_margin = margin; }
      gf::Vector2f getMargin() const noexcept { return m_margin; }

    private:
      virtual void doLayoutRequest() override;
      virtual void doLayoutAllocation() override;

    private:
      Positioning m_positioning;
      gf::Vector2f m_margin;
    };

    class ListWidget : public Widget {
    public:
      ListWidget(Widget* parent, ListModel& model);

      void setPositioning(Positioning positioning) noexcept { m_positioning = positioning; }
      Positioning getPositioning() const noexcept { return m_positioning; }

      void setMargin(gf::Vector2f margin) noexcept { m_margin = margin; }
      gf::Vector2f getMargin() const noexcept { return m_margin; }

      void setOffset(gf::Vector2f offset) noexcept { m_offset = offset; }
      gf::Vector2f getOffset() const noexcept { return m_offset; }

      void setSpacing(float spacing) noexcept { m_spacing = spacing; }
      float getSpacing() const noexcept { return m_spacing; }

    protected:
      ListModel& getModel() { return m_model; }

    private:
      virtual void doLayoutRequest() override;
      virtual void doLayoutAllocation() override;

    private:
      ListModel& m_model;
      Positioning m_positioning;
      gf::Vector2f m_margin;
      gf::Vector2f m_offset;
      float m_spacing;
    };

    /*
     * Content widget
     */

    class FrameWidget : public BoxWidget {
    public:
      FrameWidget(Widget* parent);

      virtual void render(gf::RenderTarget& target, const gf::RenderStates& states, Theme& theme) override;
    };

    class MenuWidget : public BoxWidget {
    public:
      MenuWidget(Widget* parent, const WidgetIndexScenery& scenery);

      virtual void render(gf::RenderTarget& target, const gf::RenderStates& states, Theme& theme) override;
    private:
      const WidgetIndexScenery& m_scenery;
    };

    class ChoiceWidget : public StackWidget {
    public:
      ChoiceWidget(Widget* parent, const WidgetIndexScenery& scenery);

      virtual void render(gf::RenderTarget& target, const gf::RenderStates& states, Theme& theme) override;
    private:
      const WidgetIndexScenery& m_scenery;
    };

    class CatalogueWidget : public ListWidget {
    public:
      CatalogueWidget(Widget* parent, ListModel& model, const WidgetListScenery& scenery);

      virtual void render(gf::RenderTarget& target, const gf::RenderStates& states, Theme& theme) override;
    private:
      const WidgetListScenery& m_scenery;
    };


    class LabelWidget : public Widget {
    public:
      LabelWidget(Widget* parent, std::string caption, float characterSize = 0.03f);

      virtual void render(gf::RenderTarget& target, const gf::RenderStates& states, Theme& theme) override;
    private:
      std::string m_caption;
      float m_characterSize;
    };

    class TextWidget : public Widget {
    public:
      TextWidget(Widget* parent, std::string caption, float characterSize = 0.03f, gf::Alignment alignment = gf::Alignment::Left);

      void setCaption(std::string caption) { m_caption = std::move(caption); }
      std::string getCaption() const { return m_caption; }

      virtual void render(gf::RenderTarget& target, const gf::RenderStates& states, Theme& theme) override;
    private:
      std::string m_caption;
      float m_characterSize;
      gf::Alignment m_alignment;
    };

    class DoubleTextWidget : public Widget {
    public:
      DoubleTextWidget(Widget* parent, std::string leftCaption, std::string rightCaption, float characterSize = 0.03f);

      void setLeftCaption(std::string caption) { m_leftCaption = std::move(caption); }
      std::string getLeftCaption() const { return m_leftCaption; }

      void setRightCaption(std::string caption) { m_rightCaption = std::move(caption); }
      std::string getRightCaption() const { return m_rightCaption; }

      virtual void render(gf::RenderTarget& target, const gf::RenderStates& states, Theme& theme) override;
    private:
      std::string m_leftCaption;
      std::string m_rightCaption;
      float m_characterSize;
    };

  }
}


#endif // AKGR_UI_WIDGETS_H
