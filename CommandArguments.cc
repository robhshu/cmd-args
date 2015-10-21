#include "CommandArguments.h"

#include <algorithm>
#include <iostream>
#include <iomanip>

namespace cmdargs {
bool manager::DefaultHelpCallback(const std::string &arg) {
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

manager::manager() : trailing_args_(nullptr), default_help(nullptr) {
  trailing_args_ = addlist<strlist>("", "List of trailing parameters");
  default_help = add<callback>(
      "help", "List all commands",
      std::bind(&manager::DefaultHelpCallback, this, std::placeholders::_1));
}

void manager::Register(opt &arg) {
  if (is_name_free(arg.name_)) {
    storage_.push_back(&arg);
  }
}

const t_strlist &manager::get() const { return trailing_args_->value(); }

bool manager::is_name_free(const std::string &name) const {
  t_optlist_cit cIt(storage_.begin());
  while (cIt != storage_.end()) {
    if ((*cIt)->name_ == name) {
      return false;
    }
    ++cIt;
  }

  return true;
}

bool manager::PeekArg(std::string &arg) {
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

bool manager::run(int argc, char **argv) {
  // todo: build list of required arguments met
  int arg = 1;

  error_arg_.clear();
  trailing_args_->storage_.clear();

  // todo: sort arguments by name

  while (arg < argc) {
    // Convert to string
    std::string arg_str(argv[arg]);

    if (PeekArg(arg_str)) {
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

          if (!PeekArg(val_str)) {
            ++arg;
            value = val_str;
          }
        }
      }

      std::vector<opt *>::iterator it(storage_.begin());
      while (it != storage_.end()) {
        if ((*it)->name_ == name) {
          if (!(*it)->parse(value)) {
            return false;
          }
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
      // Adding unregistered item
      trailing_args_->parse(arg_str);
    }

    ++arg;
  }

  return true;
}
}
