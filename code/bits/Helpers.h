#ifndef AKGR_HELPERS_H
#define AKGR_HELPERS_H

#include <map>
#include <string>

#include <gf/Id.h>

#include "UIData.h"

namespace akgr {

  std::string getUIMessage(const std::map<gf::Id, UIData>& data, gf::Id id);

}



#endif // AKGR_HELPERS_H
