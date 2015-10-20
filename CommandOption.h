#ifndef COMMAND_OPTION_H_
#define COMMAND_OPTION_H_

#include <string>

class CommandOption {
protected:
  friend class CommandArguments;

  std::string name_;
  std::string desc_;
  bool required_;

public:
  CommandOption() : required_(true) {}

  virtual ~CommandOption() {}
  virtual bool Parse(const std::string &) { return false; }
};

#endif
