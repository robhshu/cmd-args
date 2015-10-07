#include "CommandArguments.h"

#include <algorithm>
#include <iostream>
#include <iomanip>

bool CommandArguments::DefaultHelpCallback(const std::string &args) {
  std::vector<CommandOption *>::iterator it(storage_.begin());
  std::streamsize len(0);

  while (it != storage_.end()) {
    len = std::max(len, static_cast<std::streamsize>((*it)->name_.size()));
    ++it;
  }

  // Tabbed
  len += 2;

  it = storage_.begin();

  while (it != storage_.end()) {
    if (!(*it)->name_.empty()) {
      std::cout << std::left << std::setw(len) << (*it)->name_ << (*it)->desc_
                << std::endl;
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

bool CommandArguments::Process(const std::string &arg) {
  const char val_char('=');

  std::string::size_type val_split(arg.find_first_of(val_char));

  std::string name;
  std::string value;

  if (val_split != arg.npos) {
    name = arg.substr(0, val_split);
    value = arg.substr(val_split + sizeof(val_char));
  } else {
    name = arg;
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
    // Add unregistered item
    trailing_args_->Parse(value);
  }

  return true;
}

bool CommandArguments::ApplyArgumentList(int argc, char **argv) {
  // todo: build list of required arguments met
  int arg = 1;

  trailing_args_->storage_.clear();

  // todo: sort arguments by name

  while (arg < argc) {
    // Extract from format [--]NAME[=VAL] to NAME and [VAL]
    char *szArgPtr = argv[arg];

    if (*szArgPtr == '-') {
      ++szArgPtr;

      // Check for full name (--NAME)
      if (*szArgPtr == '-') {
        ++szArgPtr;

        if (!Process(szArgPtr)) {
          return false;
        }
      } else {
        // Single name (-NAME)

        if (!Process(szArgPtr)) {
          return false;
        }
      }
    } else {
      // Adding unregistered item
      trailing_args_->Parse(szArgPtr);
    }

    ++arg;
  }

  return true;
}
