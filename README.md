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
#include "cmd-args/CommandArguments.h"
#include <iostream>

int main(int argc, char** argv)
{
  CommandArguments Args;

  CommandArguments::FlagStorageType *flagVerbose(Args.AddFlag("verbose", "Verbose console output", false));

  if (!Args.ApplyArgumentList(argc, argv)) {
    return 0;
  }

  std::cout << "Verbose == " << flagVerbose->Get() << std::endl;

  return 0;
}
```

### Examples

Enable verbose mode:

* `main --verbose`
* `main --verbose=1`

View all supported arguments

* `main --help`

