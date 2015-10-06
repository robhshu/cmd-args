#ifndef COMMAND_ARGUMENTS_H_
#define COMMAND_ARGUMENTS_H_

#include "CommandOption.h"
#include "CommandOptionStorage.h"

#include <vector>
#include <string>
#include <functional>

class CommandArguments {
  typedef std::vector<CommandOption *> StorageType;
  typedef StorageType::const_iterator StorageTypeCIt;

public:
  typedef CommandOptionStorage<std::string> StringStorageType;
  typedef CommandOptionStorage<bool> FlagStorageType;
  typedef CommandOptionStorage<long long> NumStorageType;
  typedef CommandOptionStorage<std::function<bool(const std::string &)>>
      CallbackStorageType;

  typedef CommandOptionStorage<std::vector<std::string> > StringListStorageType;

private:
  StorageType storage_;
  CommandArguments::StringListStorageType *trailing_args_;

public:
  CommandArguments();
  ~CommandArguments();

  bool IsNameTaken(const std::string &name) const;

  StringStorageType *AddParam(const std::string &name, const std::string &desc,
                              const std::string &default_value);
  NumStorageType *AddNumber(const std::string &name, const std::string &desc,
                            long long default_value);
  FlagStorageType *AddFlag(const std::string &name, const std::string &desc,
                           bool default_value);
  CallbackStorageType *
  AddCallback(const std::string &name, const std::string &desc,
              std::function<bool(const std::string &)> callback);
  StringListStorageType *AddParamList(const std::string &name,
                                      const std::string &desc);

  const std::vector<std::string> &Get() const;

  void Register(CommandOption &arg);
  bool ApplyArgumentList(int argc, char **argv);
};

#endif
