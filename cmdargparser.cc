#include "cmdargparser.h"

namespace cmdargs {
bool parse_storage(const std::string &raw, bool &res) {
  if (raw.empty()) {
    res = true;
    return true;
  } else {
    if (raw == "0" || raw == "false") {
      res = false;
      return true;
    }

    if (raw == "1" || raw == "true") {
      res = true;
      return true;
    }
  }

  return false;
}

bool parse_storage(const std::string &raw, long long &res) {
  if (raw.empty()) {
    return false;
  } else {
    res = std::strtoll(raw.c_str(), nullptr, 10);
    return true;
  }
}

bool parse_storage(const std::string &raw, std::string &res) {
  res = raw;
  return true;
}
};
