#ifndef COMMAND_OPTION_STORAGE_H_
#define COMMAND_OPTION_STORAGE_H_

#include "CommandOptionStorageBase.h"

#include <cstdlib>

#include <string>
#include <vector>
#include <functional>

class CommandOptionFlagStorage : public CommandOptionStorageBase<bool> {
public:
  CommandOptionFlagStorage(bool default_val)
      : CommandOptionStorageBase(default_val) {}

  virtual bool Parse(const std::string &raw) {
    if (raw.empty()) {
      storage_ = true;
      return true;
    } else {
      storage_ = (raw[0] != '0');
      return true;
    }
  }
};

class CommandOptionNumStorage : public CommandOptionStorageBase<long long> {
public:
  CommandOptionNumStorage(long long default_val)
      : CommandOptionStorageBase(default_val) {}

  virtual bool Parse(const std::string &raw) {
    storage_ = std::strtoll(raw.c_str(), nullptr, 10);
    return true;
  }
};

class CommandOptionStringStorage
    : public CommandOptionStorageBase<std::string> {
public:
  CommandOptionStringStorage(const std::string &default_val)
      : CommandOptionStorageBase(default_val) {}

  virtual bool Parse(const std::string &raw) {
    storage_ = raw;
    return true;
  }
};

class CommandOptionStringListStorage
    : public CommandOptionStorageBase<std::vector<std::string>> {
public:
  virtual bool Parse(const std::string &raw) {
    storage_.push_back(raw);
    return true;
  }
};

class CommandOptionCallbackStorage
    : public CommandOptionStorageBase<
          std::function<bool(const std::string &)>> {
public:
  CommandOptionCallbackStorage(
      std::function<bool(const std::string &)> function_ptr)
      : CommandOptionStorageBase(function_ptr) {}

  virtual bool Parse(const std::string &raw) {
    if (storage_) {
      return storage_(raw);
    }
    return true;
  }
};

#endif
