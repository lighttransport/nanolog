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

#if NANOLOG_USE_FMTLIB

#include "fmt/chrono.h"
#include "fmt/core.h"

#else

#include "pprintpp.hpp"

#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#include <atomic>
#include <cstdarg>
#include <ctime>
#include <mutex>
#include <sstream>
#include <iomanip> // std::put_time

#if !defined(NANOLOG_NO_EXCEPTION_AT_FATAL)
#include <stdexcept>
#endif

#if defined(__ANDROID__) && !defined(NANOLOG_ANDROID_USE_STDIO)
#include <android/log.h>
#endif

namespace nanolog {

static enum loglevel g_level = kINFO;
static bool g_color = true;
static std::mutex g_mutex;
static std::string g_apptag;

#if defined(__ANDROID__) && !defined(NANOLOG_ANDROID_USE_STDIO)
// Android logcat prints time, so no need to print time in nanolog messsage.
static bool g_printtime = false;
#else
static bool g_printtime = true;
#endif

void set_level(enum loglevel level) { g_level = level; }

void set_color(bool enabled) { g_color = enabled; }

void set_apptag(const std::string &name) { g_apptag = name; }

void set_printtime(bool enabled) {
  g_printtime = enabled;
}

#if !defined(NANOLOG_USE_FMTLIB)
void log(int level, const char *file, const char *funcname, int line,
         const char *formatted_str, ...) {
  std::lock_guard<std::mutex> lock(g_mutex);

  if (level < g_level) {
    return;
  }

#if defined(__ANDROID__) && !defined(NANOLOG_ANDROID_USE_STDIO)

  std::string log_fmt;

  std::string header = "[" + lv_str + "] [" + file + ":" + funcname  + ":" + std::to_string(line) + "] ";

  if (g_printtime) {
    std::time_t tm = std::time(nullptr);
    std::tm ttm = *std::localtime(&tm);

    // datetime
    std::stringstream ss;
    ss << std::put_time(&ttm, "[%Y-%m-%d %H:%M:%S] ");
    std::string date_header = ss.str();

    log_fmt += date_header + header;
  } else {
    log_fmt += header;
  }

  log_fmt += std::string(fmt_str);

  std::string s = fmt::vformat(log_fmt, args);

  android_LogPriority priority = ANDROID_LOG_DEFAULT;

  if (level == kTRACE) {
    priority = ANDROID_LOG_VERBOSE;
  } else if (level == kDEBUG) {
    priority = ANDROID_LOG_DEBUG;
  } else if (level == kINFO) {
    priority = ANDROID_LOG_INFO;
  } else if (level == kWARN) {
    priority = ANDROID_LOG_WARN;
  } else if (level == kERROR) {
    priority = ANDROID_LOG_ERROR;
  } else if (level == kFATAL) {
    priority = ANDROID_LOG_FATAL;
  } else {
    priority = ANDROID_LOG_UNKNOWN;
  }

  std::string tag = g_apptag.empty() ? "nanolog" : g_apptag;

  __android_log_print(priority, tag.c_str(), "%s", s.c_str());

#else

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

  std::string header = "[" + lv_str + "] [" + file + ":" + funcname  + ":" + std::to_string(line) + "] ";
  if (!g_apptag.empty()) {
    log_fmt += "[" + g_apptag + "] ";
  }

  if (g_printtime) {
    // datetime

    std::time_t tm = std::time(nullptr);
    std::tm ttm = *std::localtime(&tm);

    std::stringstream ss;
    ss << std::put_time(&ttm, "[%Y-%m-%d %H:%M:%S] ");
    std::string date_header = ss.str();

    log_fmt += date_header + header;
  } else {
    log_fmt += header;
  }

  // append newline
  log_fmt += std::string(formatted_str) + '\n';

  va_list args;
  va_start (args, formatted_str);
  vprintf(log_fmt.c_str(), args);
  va_end (args);
#endif

  if (level == kFATAL) {
#if !defined(NANOLOG_NO_EXCEPTION_AT_FATAL)
    std::runtime_error("fatal");
#endif
  }
}
#else
void log(int level, const char *file, const char *funcname, int line,
         const char *fmt_str, fmt::format_args args) {
  std::lock_guard<std::mutex> lock(g_mutex);

  if (level < g_level) {
    return;
  }

#if defined(__ANDROID__) && !defined(NANOLOG_ANDROID_USE_STDIO)

  std::string log_fmt;

  std::time_t tm = std::time(nullptr);

  // datetime
  std::string date_header =
      fmt::format("[{:%Y-%m-%d %H:%M:%S}] ", *std::localtime(&tm));

  std::string header =
      fmt::format("[{}:{}:{}] ", file, funcname, line);
  log_fmt += date_header + header;

  log_fmt += std::string(fmt_str);

  std::string s = fmt::vformat(log_fmt, args);

  android_LogPriority priority = ANDROID_LOG_DEFAULT;

  if (level == kTRACE) {
    priority = ANDROID_LOG_VERBOSE;
  } else if (level == kDEBUG) {
    priority = ANDROID_LOG_DEBUG;
  } else if (level == kINFO) {
    priority = ANDROID_LOG_INFO;
  } else if (level == kWARN) {
    priority = ANDROID_LOG_WARN;
  } else if (level == kERROR) {
    priority = ANDROID_LOG_ERROR;
  } else if (level == kFATAL) {
    priority = ANDROID_LOG_FATAL;
  } else {
    priority = ANDROID_LOG_UNKNOWN;
  }

  std::string tag = g_apptag.empty() ? "nanolog" : g_apptag;

  __android_log_print(priority, tag.c_str(), "%s", s.c_str());

#else

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

  std::string header = fmt::format("[{}] [{}:{}:{}] ", lv_str, file, funcname, line);
  if (!g_apptag.empty()) {
    log_fmt += "[" + g_apptag + "] ";
  }

  log_fmt += date_header + header;

  // append newline
  log_fmt += std::string(fmt_str) + '\n';

  fmt::vprint(log_fmt, args);
#endif

  if (level == kFATAL) {
#if !defined(NANOLOG_NO_EXCEPTION_AT_FATAL)
    std::runtime_error("fatal");
#endif
  }
}
#endif

}  // namespace nanolog
