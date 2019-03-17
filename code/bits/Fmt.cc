#include "Fmt.h"

namespace akgr {

  std::string escapeString(const std::string& in) {
    std::string out;

    for (auto c : in) {
      switch (c) {
        case '\n':
          out.append("\\n");
          break;
        case '\t':
          out.append("\\t");
          break;
        case '\"':
          out.append("\\\"");
        default:
          out.push_back(c);
          break;
      }
    }

    return out;
  }

}
