#include "Helpers.h"

#include <cinttypes>

#include <gf/Log.h>

namespace akgr {

  std::string getUIMessage(const std::map<gf::Id, UIData>& data, gf::Id id) {
    auto it = data.find(id);

    if (it == data.end()) {
      gf::Log::error("Unknown id in UIData: %" PRIX64 "\n", id);
      return "";
    }

    return it->second.message; // TODO: add l10n
  }

}
