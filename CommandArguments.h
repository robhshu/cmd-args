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

  StorageType storage_;

  CommandOptionStringListStorage *trailing_args_;
  CommandOptionCallbackStorage *default_help;
  std::string error_arg_;

  bool DefaultHelpCallback(const std::string &arg);

  bool PeekArg(std::string &arg);

public:
  CommandArguments();
  ~CommandArguments();

  bool IsNameTaken(const std::string &name) const;

  CommandOptionStringStorage *AddParam(const std::string &name,
                                       const std::string &desc,
                                       const std::string &default_value);
  CommandOptionNumStorage *AddNumber(const std::string &name,
                                     const std::string &desc,
                                     long long default_value);
  CommandOptionFlagStorage *
  AddFlag(const std::string &name, const std::string &desc, bool default_value);
  CommandOptionCallbackStorage *
  AddCallback(const std::string &name, const std::string &desc,
              std::function<bool(const std::string &)> callback);
  CommandOptionStringListStorage *AddParamList(const std::string &name,
                                               const std::string &desc);

  const std::vector<std::string> &Get() const;

  void Register(CommandOption &arg);
  bool ApplyArgumentList(int argc, char **argv);

  const std::string &GetInvalid() const { return error_arg_; }
  bool HasInvalid() const { return !GetInvalid().empty(); }
};

#endif
