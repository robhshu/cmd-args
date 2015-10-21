#ifndef COMMAND_OPTION_STORAGE_BASE_H_
#define COMMAND_OPTION_STORAGE_BASE_H_

#include "cmdargopt.h"

namespace cmdargs {
template <class T> class storagebase : public opt {
public:
  typedef typename T StorageType;

  T storage_;

  storagebase() : opt() {}
  storagebase(T default_val) : opt(), storage_(default_val) {}
  virtual ~storagebase() {}

  const T &value() const { return storage_; }
};
};

#endif
