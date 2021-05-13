#ifndef AKGR_FLOOR_RENDERER_H
#define AKGR_FLOOR_RENDERER_H

#include <vector>

#include <gf/Ref.h>
#include <gf/RenderStates.h>
#include <gf/RenderTarget.h>

namespace akgr {


  class FloorRenderer {
  public:
    FloorRenderer(int priority)
    : m_priority(priority)
    {
    }

    virtual ~FloorRenderer();

    int getPriority() {
      return m_priority;
    }

    virtual void renderFloor(gf::RenderTarget& target, const gf::RenderStates& states, int32_t floor) = 0;

  private:
    int m_priority;
  };

  class FloorRendererContainer {
  public:

    void addFloorRenderer(FloorRenderer& renderer);

    void renderFloor(gf::RenderTarget& target, const gf::RenderStates& states, int32_t floor);

  private:
    std::vector<gf::Ref<FloorRenderer>> m_renderers;
  };

}


#endif // AKGR_FLOOR_RENDERER_H
