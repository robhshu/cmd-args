# cmd-args

[![Build Status](https://travis-ci.org/robhshu/cmd-args.svg)](https://travis-ci.org/robhshu/cmd-args)

Modern, lightweight command-line argument helper written in C++.


## Supports

* Flags (`--value`, `--value=1`, `--value=0`, `--value 1`)
* Parameters (`--key=value`, `--key value`)
* Nameless commands
* Function callbacks (used to implemented `--help`)


## Usage

### Code

```cpp
#include "cmdargmanager.h"
#include <iostream>

int main(int argc, char **argv) {
  cmdargs::manager arg_man(argc, argv);

  std::cout << arg_man.app_name() << std::endl;

  bool *argFlag = arg_man.add<bool>("my_flag", false);

  arg_man.add_cb("version", [&](const std::string &) -> bool {
    std::cout << "Version 1.0!" << std::endl;
    return false;
  });

  if (!arg_man.run()) {
    if (arg_man.has_invalid()) {
      std::cout << "Unrecognised option \"" << arg_man.last_invalid() << "\""
                << std::endl;
    }

    return 0;
  }

  std::cout << "my_flag == " << *argFlag << std::endl;

  return 0;
}


```

### Examples

Enable verbose mode:

* `main`
* `main --version`
* `main --my_flag`
* `main --my_flag=1`
* `main --my_flag 1`

View all supported arguments

* `main --help`

