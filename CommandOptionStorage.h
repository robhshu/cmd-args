#ifndef COMMAND_OPTION_STORAGE_H_
#define COMMAND_OPTION_STORAGE_H_

#include "CommandOptionStorageBase.h"

#include <cstdlib>

#include <string>
#include <vector>
#include <functional>

namespace cmdargs {
typedef bool t_flag;

class flag : public storagebase<t_flag> {
public:
  flag(t_flag default_val) : storagebase(default_val) {}

  virtual bool parse(const std::string &raw) {
    if (raw.empty()) {
      storage_ = true;
      return true;
    } else {
      storage_ = (raw[0] != '0');
      return true;
    }
  }
};

typedef long long t_num;

class num : public storagebase<t_num> {
public:
  num(t_num default_val) : storagebase(default_val) {}

  virtual bool parse(const std::string &raw) {
    storage_ = std::strtoll(raw.c_str(), nullptr, 10);
    return true;
  }
};

typedef std::string t_str;

class str : public storagebase<t_str> {
public:
  str(const t_str &default_val) : storagebase(default_val) {}

  virtual bool parse(const std::string &raw) {
    storage_ = raw;
    return true;
  }
};

typedef std::vector<t_str> t_strlist;

class strlist : public storagebase<t_strlist> {
public:
  virtual bool parse(const std::string &raw) {
    storage_.push_back(raw);
    return true;
  }
};

typedef std::function<bool(const std::string &)> t_callback;

class callback : public storagebase<t_callback> {
public:
  callback(t_callback func_ptr) : storagebase(func_ptr) {}

  virtual bool parse(const std::string &raw) {
    if (storage_) {
      return storage_(raw);
    }
    return true;
  }
};
};

#endif
