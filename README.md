# nanolog, Nanoscale logging library in C++11

nanolog is a simple, portable and nanoscale logging library in C++11.
nanolog uses fmtlib for formatted logging.

## Features

* Faster compilation time. nanolog itself uses very small amount of template code.
* Thread-safe. nanolog logging is a thread-safe.

## Supported platform

* [x] Linux
* [x] macOS
* [x] Windows
* [ ] Android, Raspberry Pi(AARCH64)
* [ ] iOS
* [ ] RISC-V(should work)

## Supported compilers

* Clang 3.9+
* GCC 5.4+
* Visual Studio 2017+
* Other C++11 compatible compilers

## Build

Just copy `nanolog/include`, `nanolog/src` and `deps/fmt` to your project folder.
Include `nanolog/src/nanolog.cc`, `deps/fmt/src/format.cc` and `deps/fmt/src/posix.cc` to your project.

## Note on logging API

New line(`\n`) character is prepended to each log message.
So you don't need to include new line character to format string.

## Example

```
#include "nanolog.h"

// logging method
NANOLOG_TRACE("The answer is {}", 42);
NANOLOG_DEBUG("The answer is {}", 42);
NANOLOG_INFO("The answer is {}", 42);
NANOLOG_WARN("The answer is {}", 42);
NANOLOG_ERROR("The answer is {}", 42);
NANOLOG_FATAL("The answer is {}", 42);

// set log level
nanolog::set_level(nanolog::kDEBUG);

// set colored output(default = true)
nanolog::set_color(false);
NANOLOG_ERROR("The answer is {}", 42);
```

## Compile options

### `NANOLOG_NO_EXCEPTION_AT_FATAL`

Do not throw an exception after fatal message output.

### `NANOLOG_NO_FMT_INCLUDE`

Do not include `fmt` header files.
This macro is useful when you want to include your own fmt files.

Example usage is:

```
#include "your_own/fmt/core.h"

#define NANOLOG_NO_FMT_INCLUDE
#include "nanolog.h"
```

## TODO

* [ ] Multiple logging instances.
* [ ] Custom stream output(e.g. to file)
* [ ] Write example for Android, iOS
* [ ] Emoji and better wide character support.

## License

nanolog is licensed under MIT license.

## Third party

* fmtlib : 2-clause BSD license.
