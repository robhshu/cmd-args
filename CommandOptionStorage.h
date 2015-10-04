#ifndef COMMAND_OPTION_STORAGE_H_
#define COMMAND_OPTION_STORAGE_H_

#include "CommandOptionStorageBase.h"

#include <string>
#include <vector>

template <class T>
class CommandOptionStorage : public CommandOptionStorageBase<T> {
public:
  CommandOptionStorage() : CommandOptionStorageBase() {}

  CommandOptionStorage(T default_val) : CommandOptionStorageBase(default_val) {}
};

template <>
class CommandOptionStorage<bool> : public CommandOptionStorageBase<bool> {
public:
  CommandOptionStorage(bool default_val)
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

template <>
class CommandOptionStorage<long long> : public CommandOptionStorageBase<long long> {
public:
  CommandOptionStorage(long long default_val)
      : CommandOptionStorageBase(default_val) {}

  virtual bool Parse(const std::string &raw) {
    storage_ = std::strtoll(raw.c_str(), nullptr, 10);
    return true;
  }
};

template <>
class CommandOptionStorage<std::string>
    : public CommandOptionStorageBase<std::string> {
public:
  CommandOptionStorage(const std::string &default_val)
      : CommandOptionStorageBase(default_val) {}

  virtual bool Parse(const std::string &raw) {
    storage_ = raw;
    return true;
  }
};

template <>
class CommandOptionStorage<std::vector<std::string>>
    : public CommandOptionStorageBase<std::vector<std::string>> {
public:
  virtual bool Parse(const std::string &raw) {
    storage_.push_back(raw);
    return true;
  }
};

#endif
