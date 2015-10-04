#include "CommandArguments.h"

CommandArguments::CommandArguments() : trailing_args_(nullptr) {
  trailing_args_ = AddParamList("", "List of trailing parameters");
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
                            unsigned int default_value) {
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

bool CommandArguments::ApplyArgumentList(int argc, char **argv) {
  // todo: build list of required arguments met
  int arg = 1;

  trailing_args_->storage_.clear();

  while (arg < argc) {

    // cleanup arg
    // strip --NAME=VAL to NAME
    char *szArgPtr = argv[arg];
    while (*szArgPtr == '-')
      ++szArgPtr;

    char *szValPtr = szArgPtr;
    while (*szValPtr && *szValPtr != '=')
      ++szValPtr;

    if (*szValPtr) {
      // Cut out name
      *szValPtr = 0;

      // Leave arg value
      ++szValPtr;
    } else {
      // Reset value to full name
      szValPtr = szArgPtr;
    }

    std::vector<CommandOption *>::iterator it(storage_.begin());
    while (it != storage_.end()) {

      if ((*it)->name_ == szArgPtr) {
        if (!(*it)->Parse(szValPtr)) {
          return false;
        }

        break;
      }

      ++it;
    }

    if (it == storage_.end()) {
      // Adding unregistered item
      trailing_args_->Parse(szValPtr);
    }

    ++arg;
  }

  // warning if trailing arguments are listed?

  return true;
}
