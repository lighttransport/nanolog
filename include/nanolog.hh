/*
MIT License

Copyright (c) 2019 Light Transport Entertainment Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef NANOLOG_HH_
#define NANOLOG_HH_

#if !defined(NANOLOG_NO_FMT_INCLUDE)

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#endif

// TODO(LTE): Only import fmt::format_args and fmt::make_format_args
#include "fmt/core.h"

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif

namespace nanolog {

constexpr const char *kVERSION = "0.7.0";

enum loglevel { kTRACE, kDEBUG, kINFO, kWARN, kERROR, kFATAL };

void set_level(enum loglevel level);
void set_color(bool enabled);
void set_apptag(const std::string &apptag);

void log(int level, const char *filename, const char *funcname, int line,
         const char *fmt, fmt::format_args args);

template <typename... Args>
void logger(int level, const char *filename, const char *funcname, int line,
            const char *fmt, const Args &... args) {
  log(level, filename, funcname, line, fmt, fmt::make_format_args(args...));
}

#define NANOLOG_TRACE(...)                                         \
  do {                                                             \
    nanolog::logger(nanolog::kTRACE, __FILE__, __func__, __LINE__, \
                    __VA_ARGS__);                                  \
  } while (0)

#define NANOLOG_DEBUG(...)                                         \
  do {                                                             \
    nanolog::logger(nanolog::kDEBUG, __FILE__, __func__, __LINE__, \
                    __VA_ARGS__);                                  \
  } while (0)

#define NANOLOG_INFO(...)                                         \
  do {                                                            \
    nanolog::logger(nanolog::kINFO, __FILE__, __func__, __LINE__, \
                    __VA_ARGS__);                                 \
  } while (0)

#define NANOLOG_WARN(...)                                         \
  do {                                                            \
    nanolog::logger(nanolog::kWARN, __FILE__, __func__, __LINE__, \
                    __VA_ARGS__);                                 \
  } while (0)

#define NANOLOG_ERROR(...)                                         \
  do {                                                             \
    nanolog::logger(nanolog::kERROR, __FILE__, __func__, __LINE__, \
                    __VA_ARGS__);                                  \
  } while (0)

#define NANOLOG_FATAL(...)                                         \
  do {                                                             \
    nanolog::logger(nanolog::kFATAL, __FILE__, __func__, __LINE__, \
                    __VA_ARGS__);                                  \
  } while (0)

}  // namespace nanolog

#endif  // NANOLOG_HH_
