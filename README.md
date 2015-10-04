# cmd-args

Modern, lightweight command-line argument helper written in C++.


## Supports

* Flags (`--value`, `--value=1`, `--value=0`)
* Parameters (`--key=value`)
* Nameless commands


## Usage

```cpp
#include "CommandArguments.h"
#include <stdio.h>

int main(int argc, char** argv)
{
  CommandArguments Args;

  CommandArguments::FlagStorageType *flagVerbose(Args.AddFlag("verbose", "Verbose console output", false));

  if (!Args.ApplyArgumentList(argc, argv)) {
    printf("Failed to read argument list\n");
    return 0;
  }

  printf("Verbose == %s", flagVerbose->Get() ? "true" : "false");

  return 0;
}
```
