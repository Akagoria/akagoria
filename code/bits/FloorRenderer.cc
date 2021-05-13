#include "FloorRenderer.h"

namespace akgr {

  /*
   * FloorRenderer
   */

  FloorRenderer::~FloorRenderer() = default;

  /*
   * FloorRendererContainer
   */

  void FloorRendererContainer::addFloorRenderer(FloorRenderer& renderer) {
    m_renderers.push_back(renderer);
  }

  void FloorRendererContainer::renderFloor(gf::RenderTarget& target, const gf::RenderStates& states, int32_t floor) {
    for (FloorRenderer& renderer : m_renderers) {
      renderer.renderFloor(target, states, floor);
    }
  }

}
