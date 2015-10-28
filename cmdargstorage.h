#ifndef CMDARG_STORAGE_H_
#define CMDARG_STORAGE_H_

#include "cmdargstoragebase.h"
#include "cmdargparser.h"

#include <string>
#include <vector>
#include <functional>

namespace cmdargs {
typedef bool t_flag;
typedef long long t_num;
typedef std::string t_str;
typedef std::function<bool(const std::string &)> t_callback;

template <class T> class storage : public storagebase<T> {
public:
  storage(T default_val) : storagebase(default_val) {}

  virtual bool parse(const std::string &raw) {
    return parse_storage(raw, storage_);
  }
};

template <> class storage<t_callback> : public storagebase<t_callback> {
public:
  storage(t_callback default_val) : storagebase(default_val) {}

  virtual bool parse(const std::string &raw) { return storage_(raw); }
};

template <class T> class multistorage : public storagebase<std::vector<T>> {
public:
  virtual bool parse(const std::string &raw) {
    T tmp;
    if (parse_storage(raw, tmp)) {
      storage_.push_back(tmp);
      return true;
    }

    return false;
  }
};
}

#endif
