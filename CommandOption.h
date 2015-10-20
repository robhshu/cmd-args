#ifndef COMMAND_OPTION_H_
#define COMMAND_OPTION_H_

#include <string>

namespace cmdargs {
class opt {
protected:
  friend class manager;

  std::string name_;
  std::string desc_;
  bool required_;

public:
  opt() : required_(true) {}

  virtual ~opt() {}
  virtual bool parse(const std::string &) { return false; }
};
};

#endif
