#include "CommandArguments.h"
#include <iostream>

int main(int argc, char **argv) {
  CommandArguments Args;

  CommandArguments::FlagStorageType *flagVerbose(
      Args.AddFlag("verbose", "Verbose console output", false));

  if (!Args.ApplyArgumentList(argc, argv)) {
    if (Args.HasInvalid()) {
      std::cout << "Unrecognised option \"" << Args.GetInvalid() << "\""
                << std::endl;
    }
    return 0;
  }

  std::cout << "Verbose == " << flagVerbose->Get() << std::endl;

  return 0;
}
