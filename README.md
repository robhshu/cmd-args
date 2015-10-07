# cmd-args

[![Build Status](https://travis-ci.org/robhshu/cmd-args.svg)](https://travis-ci.org/robhshu/cmd-args)

Modern, lightweight command-line argument helper written in C++.


## Supports

* Flags (`--value`, `--value=1`, `--value=0`)
* Parameters (`--key=value`)
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

* Set the verbose flag to true using `main --verbose` or `main --verbose=1`. The value by default is `false`.
* View all supported arguments using `main --help`
