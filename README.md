# nanolog, Nanoscale logging library in C++11

nanolog is a simple, portable and nanoscale logging library in C++11.
nanolog uses `pprintpp`(default) or `fmtlib` for supporting Python-like format string(`{}`).

## Features

* Faster compilation time. nanolog itself uses very small amount of template code.
  * Default uses `pprintpp` backend, which is fast to compile than `fmtlib`.
    * But has many limitations(see the `Limitation` section in the below)
    * Using `pprintpp` is rougly 3x ~ 10x faster compile than `fmtlib`(when `-O2` optimization enabled)
  * fmtlib backend is also provided.
* Thread-safe. nanolog logging is a thread-safe.

nanolog is good if you want faster C++11 compile time but don't need absolute performance of logging output.
e.g. graphics, raytracing, machine learning application. An application where text logging is important for data loading/saving, but non-text debugging is primarily used in its primary task.

## Supported platform

* [x] Linux 64bit
* [x] macOS
* [x] Windows 10 64bit
* [x] Android arm64va8
* [x] Raspberry Pi(AARCH64)
* [ ] iOS(Should work)
* [ ] RISC-V(should work)

## Supported compilers

* C++11 compiler
  * Clang 3.9+
  * GCC 5.4+
  * Visual Studio 2017+
  * Other C++11 compatible compilers

## Build

Just copy `nanolog/include`, `nanolog/src` and `deps/pprintpp` or `deps/fmtlib` to your project folder and add .cc and .hh for your project.

### pprintpp backend

Add `nanolog/src/nanolog.cc` to your project.
(pprintpp is a header only library, so no extra .cc required)

#### Limitation

Single argument would give compilation error(due to C++'s variadic macro specification) when you raise the compiler warning level.

```
NANOLOG_INFO("hello");   // => error

NANOLOG_INFO("{}", "hello"); // => ok
```

Cannot specify `std::string` directly.

```
std::string str = "hello";
NANOLOG_INFO("{}", str);   // => error

NANOLOG_INFO("{}", str.c_str()); // => ok
```

### fmtlib backend

Add `nanolog/src/nanolog.cc`, `deps/fmt/src/format.cc` and `deps/fmt/src/posix.cc` to your project.

### Build test on Visual Studio 2017

Install meson.
(Assume meson will be installed to `C:\Program Files\Meson\meson.exe`)

Edit path in `vcsetup.bat` if required.

Open `Developer Command Prompt for Visual Studio 2017` from Windows menu.

```
> cd $nanolog
> vcsetup.bat
```

VS solution file will be generated at `build` directory.

## Note on logging API

New line(`\n`) character is prepended to each log message.
So you don't need to include new line character to format string.

## Example

```
#include "nanolog.hh"

// logging method
NANOLOG_TRACE("The answer is {}", 42);
NANOLOG_DEBUG("The answer is {}", 42);
NANOLOG_INFO("The answer is {}", 42);
NANOLOG_WARN("The answer is {}", 42);
NANOLOG_ERROR("The answer is {}", 42);
NANOLOG_FATAL("The answer is {}", 42);

// set log level
nanolog::set_level(nanolog::kDEBUG);

// set app tag(useful for Android)
nanolog::set_apptag("myapp");

// print time(disabled by default for Android logcat)
nanolog::set_printtime(false);

// set colored output(default = true. NOTE: to see colored output on Android logcat, you may need to see it on Android Studio)
nanolog::set_color(false);
NANOLOG_ERROR("The answer is {}", 42);
```

## Compile options

### `NANOLOG_NO_EXCEPTION_AT_FATAL`

Do not throw an exception after fatal message output.

### fmtlib backend

Use `NANOLOG_USE_FMTLIB` compile flags to use fmtlib backend.

#### `NANOLOG_NO_FMT_INCLUDE`

Do not include `fmt` header files in `nanolog.h`.
This macro is useful when you want to include your own fmt files.
(fmtlib is required to build nanolog anyway)

Example usage is:

```
#include "your_own/fmt/core.h"

#define NANOLOG_USE_FMTLIB
#define NANOLOG_NO_FMT_INCLUDE
#include "nanolog.hh"
```


### `NANOLOG_ANDROID_USE_STDIO`

Print log to stdout for Android platform.
In default, log messages are sent to Android log system(`adb logcat` to see it).
When `NANOLOG_ANDROID_USE_STDIO` is defined, log messeages are sent to `stdout`.
This flags is useful when you build standalone native Android program(`./a.out` style app. for example unit tester)

## TODO

* [ ] Multiple logging instances.
* [ ] Custom stream output(e.g. to file)
* [ ] Write example for Android, iOS
* [ ] Emoji and better wide character support.

## License

nanolog is licensed under MIT license.

## Third party

* pprintpp: MIT or simplified BSD(2-clause BSD?) license. https://github.com/wolever/pprintpp
* fmtlib : 2-clause BSD license. https://github.com/fmtlib/fmt
