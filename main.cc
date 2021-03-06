﻿#include "cmdargmanager.h"
#include <iostream>

int main(int argc, char **argv) {
  cmdargs::manager arg_man(argc, argv);

  // Output the application startup name
  std::cout << arg_man.app_name() << std::endl;

  // Create a boolean argument
  bool *argFlag = arg_man.add<bool>("my_flag", false);

  // Create callback to output a version string to the console
  arg_man.add_cb("version", [&](const std::string &) -> bool {
    std::cout << "Version 1.0!" << std::endl;
    return false;
  });

  // The result of run returns if execution should continue or not
  if (!arg_man.run()) {
    // If we received an invalid argument, output it
    if (arg_man.has_invalid()) {
      std::cout << "Unrecognised option \"" << arg_man.last_invalid() << "\""
                << std::endl;
    }

    // Execution can be marked to not continue from a callback
    return 0;
  }

  // Output the final boolean value
  std::cout << "my_flag == " << *argFlag << std::endl;

  return 0;
}
