#ifndef CMDARG_STORAGEBASE_H_
#define CMDARG_STORAGEBASE_H_

#include "cmdargopt.h"

namespace cmdargs {
template <class T> class storagebase : public opt {
public:
  T storage_;

  storagebase() : opt() {}
  storagebase(T default_val) : opt(), storage_(default_val) {}
  virtual ~storagebase() {}

  const T &value() const { return storage_; }
};
};

#endif
