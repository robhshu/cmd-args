#ifndef COMMAND_ARGUMENTS_H_
#define COMMAND_ARGUMENTS_H_

#include "CommandOption.h"
#include "CommandOptionStorage.h"

#include <vector>
#include <string>
#include <functional>

namespace cmdargs {
class manager {
  typedef std::vector<opt *> t_optlist;
  typedef t_optlist::const_iterator t_optlist_cit;

  t_optlist storage_;

  strlist *trailing_args_;
  callback *default_help;

  std::string error_arg_;

  bool DefaultHelpCallback(const std::string &arg);

  bool PeekArg(std::string &arg);
  void Register(opt &arg);

public:
  manager();

  bool is_name_free(const std::string &name) const;

  template <typename T, typename TStorage = T::StorageType>
  T *add(const std::string &name, const std::string &desc,
         const TStorage &default_value) {
    T *val(nullptr);

    if (is_name_free(name)) {
      val = new T(default_value);
      val->name_ = name;
      val->desc_ = desc;

      Register(*val);
    }

    return val;
  }

  template <typename T>
  T *addlist(const std::string &name, const std::string &desc) {
    T *val(nullptr);

    if (is_name_free(name)) {
      val = new T();
      val->name_ = name;
      val->desc_ = desc;

      Register(*val);
    }

    return val;
  }

  const t_strlist &get() const;

  bool run(int argc, char **argv);

  const std::string &last_invalid() const { return error_arg_; }
  bool has_invalid() const { return last_invalid().empty(); }
};
}

#endif
