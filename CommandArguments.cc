#include "CommandArguments.h"

#include <algorithm>
#include <iostream>
#include <iomanip>

bool CommandArguments::DefaultHelpCallback(const std::string &arg) {
  const std::streamsize pad_len(2);

  // Try to lookup help for the named argument

  if (!arg.empty()) {
    std::vector<CommandOption *>::iterator it(storage_.begin());

    while (it != storage_.end()) {
      if ((*it)->name_ == arg) {
        std::streamsize len(static_cast<std::streamsize>((*it)->name_.size()));

        std::cout << std::left << std::setw(pad_len + len) << (*it)->name_
                  << (*it)->desc_ << std::endl;

        return false;
      }

      ++it;
    }
  }

  // Iterate over all argument names to find the longest (for formatting)

  std::streamsize len(0);

  std::vector<CommandOption *>::iterator it(storage_.begin());
  while (it != storage_.end()) {
    len = std::max(len, static_cast<std::streamsize>((*it)->name_.size()));
    ++it;
  }

  // Dump all valid arguments (non-zero), with their description

  it = storage_.begin();
  while (it != storage_.end()) {
    if (!(*it)->name_.empty()) {
      std::cout << std::left << std::setw(pad_len + len) << (*it)->name_
                << (*it)->desc_ << std::endl;
    }
    ++it;
  }

  return false;
}

CommandArguments::CommandArguments()
    : trailing_args_(nullptr), default_help(nullptr) {
  trailing_args_ = AddParamList("", "List of trailing parameters");
  default_help = AddCallback("help", "List all commands",
                             std::bind(&CommandArguments::DefaultHelpCallback,
                                       this, std::placeholders::_1));
}

CommandArguments::~CommandArguments() {}

void CommandArguments::Register(CommandOption &arg) {
  if (!IsNameTaken(arg.name_)) {
    storage_.push_back(&arg);
  }
}

const std::vector<std::string> &CommandArguments::Get() const {
  return trailing_args_->Get();
}

bool CommandArguments::IsNameTaken(const std::string &name) const {
  StorageTypeCIt cIt(storage_.begin());
  while (cIt != storage_.end()) {
    if ((*cIt)->name_ == name) {
      return true;
    }
    ++cIt;
  }

  return false;
}

CommandArguments::StringStorageType *
CommandArguments::AddParam(const std::string &name, const std::string &desc,
                           const std::string &default_value) {
  CommandArguments::StringStorageType *val(nullptr);

  if (!IsNameTaken(name)) {
    val = new CommandArguments::StringStorageType(default_value);
    val->name_ = name;
    val->desc_ = desc;

    Register(*val);
  }

  return val;
}

CommandArguments::NumStorageType *
CommandArguments::AddNumber(const std::string &name, const std::string &desc,
                            long long default_value) {
  CommandArguments::NumStorageType *val(nullptr);

  if (!IsNameTaken(name)) {
    val = new CommandArguments::NumStorageType(default_value);
    val->name_ = name;
    val->desc_ = desc;

    Register(*val);
  }

  return val;
}

CommandArguments::FlagStorageType *
CommandArguments::AddFlag(const std::string &name, const std::string &desc,
                          bool default_value) {
  CommandArguments::FlagStorageType *val(nullptr);

  if (!IsNameTaken(name)) {
    val = new CommandArguments::FlagStorageType(default_value);
    val->name_ = name;
    val->desc_ = desc;

    Register(*val);
  }

  return val;
}

CommandArguments::CallbackStorageType *CommandArguments::AddCallback(
    const std::string &name, const std::string &desc,
    std::function<bool(const std::string &)> callback) {
  CommandArguments::CallbackStorageType *val(nullptr);

  if (!IsNameTaken(name)) {
    val = new CommandArguments::CallbackStorageType(callback);
    val->name_ = name;
    val->desc_ = desc;

    Register(*val);
  }

  return val;
}

CommandArguments::StringListStorageType *
CommandArguments::AddParamList(const std::string &name,
                               const std::string &desc) {
  CommandArguments::StringListStorageType *val(nullptr);

  if (!IsNameTaken(name)) {
    val = new CommandArguments::StringListStorageType();
    val->name_ = name;
    val->desc_ = desc;

    Register(*val);
  }

  return val;
}

bool CommandArguments::PeekArg(std::string &arg) {
  const char tack_char('-');
  std::string::size_type tack_pos(arg.find_first_not_of(tack_char));

  // Only allow double-tacks for now (--arg)
  if (tack_pos == 2) {
    arg = arg.substr(2);
    return true;
  }

  return false;
}

bool CommandArguments::ApplyArgumentList(int argc, char **argv) {
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

      std::vector<CommandOption *>::iterator it(storage_.begin());
      while (it != storage_.end()) {
        if ((*it)->name_ == name) {
          if (!(*it)->Parse(value)) {
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
      trailing_args_->Parse(arg_str);
    }

    ++arg;
  }

  return true;
}
