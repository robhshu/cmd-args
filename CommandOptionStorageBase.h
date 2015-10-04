#ifndef COMMAND_OPTION_STORAGE_BASE_H_
#define COMMAND_OPTION_STORAGE_BASE_H_

#include "CommandOption.h"

template <class T> class CommandOptionStorageBase : public CommandOption {
public:
  T storage_;

  CommandOptionStorageBase() : CommandOption() {}

  CommandOptionStorageBase(T default_val)
      : CommandOption(), storage_(default_val) {}

  virtual ~CommandOptionStorageBase() {}

  const T &Get() const { return storage_; }
};

#endif
