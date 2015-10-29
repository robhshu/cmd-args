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

  virtual bool on_set(const std::string &raw) {
    touched_ = parse_storage(raw, storage_);
    return touched_;
  }
};

template <> class storage<t_callback> : public storagebase<t_callback> {
public:
  storage(t_callback default_val) : storagebase(default_val) {}

  virtual bool on_set(const std::string &raw) {
    // Minor variant here. Callbacks are always touched, the success depends on
    // the function.
    touched_ = true;
    return storage_(raw);
  }
};

template <class T> class multistorage : public storagebase<std::vector<T>> {
public:
  virtual bool on_set(const std::string &raw) {
    T tmp;
    if (parse_storage(raw, tmp)) {
      touched_ = true;
      storage_.push_back(tmp);
      return true;
    }

    return false;
  }
};
}

#endif
