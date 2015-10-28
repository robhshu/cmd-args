#ifndef CMDARG_OPT_H_
#define CMDARG_OPT_H_

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

  const std::string &name() const { return name_; }
  const std::string &desc() const { return desc_; }
};
};

#endif
