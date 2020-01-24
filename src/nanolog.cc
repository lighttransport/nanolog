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
#include "nanolog.hh"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#endif

#include "fmt/chrono.h"
#include "fmt/core.h"

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#include <atomic>
#include <cstdarg>
#include <ctime>
#include <mutex>

#if !defined(NANOLOG_NO_EXCEPTION_AT_FATAL)
#include <stdexcept>
#endif

namespace nanolog {

static enum loglevel g_level = kINFO;
static bool g_color = true;
static std::mutex g_mutex;

void set_level(enum loglevel level) { g_level = level; }

void set_color(bool enabled) { g_color = enabled; }

void log(int level, const char *file, const char *funcname, int line,
         const char *fmt_str, fmt::format_args args) {
  std::lock_guard<std::mutex> lock(g_mutex);

  if (level < g_level) {
    return;
  }

  std::string log_fmt;

  std::string lv_str;
  if (level == kTRACE) {
    lv_str = "trace";
  } else if (level == kDEBUG) {
    lv_str = "debug";
  } else if (level == kINFO) {
    lv_str = "info";
  } else if (level == kWARN) {
    if (g_color) {
      lv_str = "\033[33mwarn\033[m";
    } else {
      lv_str = "warn";
    }
  } else if (level == kERROR) {
    if (g_color) {
      lv_str = "\033[31merror\033[m";
    } else {
      lv_str = "error";
    }
  } else if (level == kFATAL) {
    if (g_color) {
      lv_str = "\033[31m\033[1mfatal\033[m";
    } else {
      lv_str = "fatal";
    }
  } else {
    lv_str = "UNKNOWN";
  }

  std::time_t tm = std::time(nullptr);

  // datetime
  std::string date_header =
      fmt::format("[{:%Y-%m-%d %H:%M:%S}] ", *std::localtime(&tm));

  std::string header =
      fmt::format("[{}] [{}:{}:{}] ", lv_str, file, funcname, line);
  log_fmt += date_header + header;

  // append newline
  log_fmt += std::string(fmt_str) + '\n';

  fmt::vprint(log_fmt, args);

  if (level == kFATAL) {
#if !defined(NANOLOG_NO_EXCEPTION_AT_FATAL)
    std::runtime_error("fatal");
#endif
  }
}

}  // namespace nanolog
