#ifndef AKGR_WORLD_H
#define AKGR_WORLD_H

namespace akgr {

  struct World {
    WorldData data;
    WorldState state;
    WorldScenery scenery;
    Script script;

    World();
    void bind();
  };

}


#endif // AKGR_WORLD_H
