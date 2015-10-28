#ifndef CMDARG_MANAGER_H_
#define CMDARG_MANAGER_H_

#include "cmdargopt.h"
#include "cmdargstorage.h"

#include <vector>
#include <list>
#include <string>
#include <functional>

namespace cmdargs {
class manager {
  typedef std::vector<opt *> t_optlist;
  typedef t_optlist::const_iterator t_optlist_cit;

  t_optlist storage_;

  std::vector<t_str> trailing_args_;

  std::string app_name_;
  std::string error_arg_;

  bool help_callback(const std::string &arg);

  bool peek_arg(std::string &arg);
  opt *register_arg(opt *arg);

  bool is_name_free(const std::string &name) const;

public:
  manager();
  ~manager();

  template <typename T>
  T *add(const std::string &name, const T &default_value,
         bool required = true) {
    opt *val(register_arg(new storage<T>(default_value)));

    if (is_name_free(name)) {
      val->name_ = name;
      val->desc_ = "";
      val->required_ = required;
    }

    return &reinterpret_cast<storage<T> *>(val)->storage_;
  }

  template <typename T>
  std::vector<T> *add_vec(const std::string &name, bool required = true) {
    opt *val(register_arg(new multistorage<T>()));

    if (is_name_free(name)) {
      val->name_ = name;
      val->desc_ = "";
      val->required_ = required;
    }

    return &reinterpret_cast<multistorage<T> *>(val)->storage_;
  }

  void add_cb(const std::string &name, t_callback method, bool required = true);

  bool run(int argc, char **argv);

  const std::vector<t_str> get_commands() const { return trailing_args_; }

  const std::string &app_name() const { return app_name_; }

  const std::string &last_invalid() const { return error_arg_; }
  bool has_invalid() const { return !last_invalid().empty(); }
};
}

#endif
