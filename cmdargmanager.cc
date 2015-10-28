#include "cmdargmanager.h"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <tuple>

namespace cmdargs {
bool manager::help_callback(const std::string &arg) {
  const std::streamsize pad_len(2);

  // Try to lookup help for the named argument

  if (!arg.empty()) {
    t_optlist_cit cIt(storage_.begin());

    while (cIt != storage_.end()) {
      if ((*cIt)->name() == arg) {
        std::streamsize len(
            static_cast<std::streamsize>((*cIt)->name().size()));

        std::cout << std::left << std::setw(pad_len + len) << (*cIt)->name()
                  << (*cIt)->desc() << std::endl;

        return false;
      }

      ++cIt;
    }
  }

  // Iterate over all argument names to find the longest (for formatting)

  std::streamsize len(0);

  t_optlist_cit cIt(storage_.begin());
  while (cIt != storage_.end()) {
    len = std::max(len, static_cast<std::streamsize>((*cIt)->name().size()));
    ++cIt;
  }

  // Dump all valid arguments (non-zero), with their description

  cIt = storage_.begin();
  while (cIt != storage_.end()) {
    if (!(*cIt)->name().empty()) {
      std::cout << std::left << std::setw(pad_len + len) << (*cIt)->name()
                << (*cIt)->desc() << std::endl;
    }
    ++cIt;
  }

  return false;
}

manager::manager() {

  add_cb("help",
         std::bind(&manager::help_callback, this, std::placeholders::_1),
         false);
}

manager::~manager() {
  t_optlist_cit cIt(storage_.begin());
  while (cIt != storage_.end()) {
    delete *cIt;
    ++cIt;
  }

  storage_.clear();
}

opt *manager::register_arg(opt *arg) {
  if (arg != nullptr) {
    storage_.push_back(arg);
  }

  return arg;
}

bool manager::is_name_free(const std::string &name) const {
  if (name.empty()) {
    return false;
  }

  t_optlist_cit cIt(storage_.begin());
  while (cIt != storage_.end()) {
    if ((*cIt)->name_ == name) {
      return false;
    }
    ++cIt;
  }

  return true;
}

bool manager::peek_arg(std::string &arg) {
  const char tack_char('-');
  std::string::size_type tack_pos(arg.find_first_not_of(tack_char));

  // Allow double-tacks (--arg)
  if (tack_pos == 2) {
    arg = arg.substr(2);
    return true;
  }

  const char switch_char('/');

  // Allow switch-style arguments (/arg)
  if (!arg.empty()) {
    if (arg[0] == switch_char) {
      arg = arg.substr(1);
      return true;
    }
  }

  return false;
}

void manager::add_cb(const std::string &name, t_callback method,
                     bool required) {
  opt *val(register_arg(new storage<t_callback>(method)));

  if (is_name_free(name)) {
    val->name_ = name;
    val->desc_ = "";
    val->required_ = required;
  }
}

void manager::set_app_name(const std::string &name) {
  const char path_sep_char('\\');
  std::string::size_type path_sep_pos(name.find_last_of(path_sep_char));

  if (path_sep_pos != name.npos) {
    app_name_ = name.substr(path_sep_pos + 1);
  } else {
    app_name_ = name;
  }
}

bool manager::run(int argc, char **argv) {

  set_app_name(std::string(argv[0]));

  error_arg_.clear();
  trailing_args_.clear();

  typedef std::tuple<t_optlist_cit, std::string> ArgList;
  std::vector<ArgList> final_args;

  int arg(1);
  while (arg < argc) {
    // Convert to string
    std::string arg_str(argv[arg]);

    if (peek_arg(arg_str)) {
      const char val_char('=');
      std::string::size_type split_pos(arg_str.find_first_of(val_char, 1));

      std::string name;
      std::string value;

      if (split_pos != arg_str.npos) {
        name = arg_str.substr(0, split_pos);
        value = arg_str.substr(split_pos + sizeof(val_char));
      } else {
        name = arg_str;

        // Try to determine the value using the next argument

        if (arg + 1 < argc) {
          std::string val_str(argv[arg + 1]);

          if (!peek_arg(val_str)) {
            ++arg;
            value = val_str;
          }
        }
      }

      t_optlist_cit it(storage_.begin());
      while (it != storage_.end()) {
        if ((*it)->name_ == name) {

          final_args.push_back(std::make_tuple(it, value));

          break;
        }

        ++it;
      }

      if (it == storage_.end()) {
        // Incorrect usage; unrecognised argument
        error_arg_ = name;
        return false;
      }

    } else {
      // Unregistered commands are retained
      trailing_args_.push_back(arg_str);
    }

    ++arg;
  }

  if (has_invalid()) {
    return false;
  } else {

    std::vector<ArgList>::const_iterator argIt = final_args.begin();
    while (argIt != final_args.end()) {
      t_optlist_cit item;
      std::string opt_arg;

      std::tie(item, opt_arg) = *argIt;
      (*item)->parse(opt_arg);

      ++argIt;
    }

    return true;
  }
}
}
