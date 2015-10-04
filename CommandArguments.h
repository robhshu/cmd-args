#ifndef COMMAND_ARGUMENTS_H_
#define COMMAND_ARGUMENTS_H_

#include "CommandOption.h"
#include "CommandOptionStorage.h"

#include <vector>
#include <string>

class CommandArguments {
  typedef std::string StringType;
  typedef bool FlagType;

  typedef std::vector<CommandOption *> StorageType;
  typedef StorageType::const_iterator StorageTypeCIt;

public:
  typedef CommandOptionStorage<StringType> StringStorageType;
  typedef CommandOptionStorage<bool> FlagStorageType;
  typedef CommandOptionStorage<unsigned int> NumStorageType;
  typedef CommandOptionStorage<signed int> SignedNumStorageType;

  typedef CommandOptionStorage<std::vector<StringType>> StringListStorageType;

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
                            unsigned int default_value);
  FlagStorageType *AddFlag(const std::string &name, const std::string &desc,
                           bool default_value);
  StringListStorageType *AddParamList(const std::string &name,
                                      const std::string &desc);

  const std::vector<std::string> &Get() const;

  void Register(CommandOption &arg);
  bool ApplyArgumentList(int argc, char **argv);
};

#endif
