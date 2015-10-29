#ifndef CMDARG_OPT_H_
#define CMDARG_OPT_H_

#include <string>

namespace cmdargs {
class opt {
protected:
  friend class manager;

  std::string name_;
  std::string desc_;
  bool touched_;
  bool required_;

public:
  opt() : touched_(false), required_(true) {}

  virtual ~opt() {}
  virtual bool on_set(const std::string &) { return false; }

  virtual bool is_valid() { return touched_; }
  const bool is_required() const { return required_; }

  const std::string &get_name() const { return name_; }
  const std::string &get_desc() const { return desc_; }
};
};

#endif
