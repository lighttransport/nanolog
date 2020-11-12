/*
MIT License

Copyright (c) 2019 - 2020 Light Transport Entertainment Inc.

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

#include <string>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#endif

#ifdef NANOLOG_USE_FMTLIB

#if !defined(NANOLOG_NO_FMT_INCLUDE)

// TODO(LTE): Only import fmt::format_args and fmt::make_format_args
#include "fmt/core.h"

#endif

#elif NANOLOG_USE_PPRINTPP

#include "pprintpp.hpp"

#else
// internal
#include <sstream>
#include <vector>
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

namespace nanolog {

constexpr const char *kVERSION = "0.9.0";

enum loglevel { kTRACE, kDEBUG, kINFO, kWARN, kERROR, kFATAL };

void set_level(enum loglevel level);
void set_color(bool enabled);
void set_apptag(const std::string &apptag);

// Show current time?(default = false for Android logcat, true for other
// platform. Disabling print time may give faster logging)
void set_printtime(bool enabled);

#if defined(NANOLOG_USE_PPRINTPP)

//
// Limitation: when using pprintpp, zero or single argument is not allowed since
// empty __VA_ARGS__ is invalid in strict manner. e.g. NANOLOG_INFO("hello")
// gives an error
//
// AUTOFORMAT in pprintpp.hpp failed to compile on msvc, so provide our own
// portable one

//
// Suppress some compiler warnings as a work around
//

#ifdef __clang__
#define NANOLOG_AUTOFORMAT(x, s, ...)                              \
  _Pragma("clang diagnostic push") \
  _Pragma("clang diagnostic ignored \"-Wnon-pod-varargs\"") \
  _Pragma("clang diagnostic ignored \"-Wdouble-promotion\"") \
  {                                                                \
    struct strprov {                                               \
      static constexpr const char *str() { return s; }             \
    };                                                             \
    using paramtypes = decltype(pprintpp::tie_types(__VA_ARGS__)); \
    using af = pprintpp::autoformat_t<strprov, paramtypes>;        \
    x = af::str();                                                 \
  } \
  _Pragma("clang diagnostic pop")
#else
#define NANOLOG_AUTOFORMAT(x, s, ...)                              \
  {                                                                \
    struct strprov {                                               \
      static constexpr const char *str() { return s; }             \
    };                                                             \
    using paramtypes = decltype(pprintpp::tie_types(__VA_ARGS__)); \
    using af = pprintpp::autoformat_t<strprov, paramtypes>;        \
    x = af::str();                                                 \
  }
#endif

void log(int level, const char *filename, const char *funcname, int line,
         const char *formatted_str, ...);

#ifdef __clang__
#define NANOLOG_TRACE(s, ...)                              \
  _Pragma("clang diagnostic push") \
  _Pragma("clang diagnostic ignored \"-Wnon-pod-varargs\"") \
  _Pragma("clang diagnostic ignored \"-Wdouble-promotion\"") \
  do {                                                               \
    const char *fmt;                                                 \
    NANOLOG_AUTOFORMAT(fmt, s, __VA_ARGS__)                          \
    nanolog::log(nanolog::kTRACE, __FILE__, __func__, __LINE__, fmt, \
                 __VA_ARGS__);                                       \
  } while (0) \
  _Pragma("clang diagnostic pop")
#else
#define NANOLOG_TRACE(s, ...)                                        \
  do {                                                               \
    const char *fmt;                                                 \
    NANOLOG_AUTOFORMAT(fmt, s, __VA_ARGS__)                          \
    nanolog::log(nanolog::kTRACE, __FILE__, __func__, __LINE__, fmt, \
                 __VA_ARGS__);                                       \
  } while (0)
#endif

#ifdef __clang__
#define NANOLOG_DEBUG(s, ...)                              \
  _Pragma("clang diagnostic push") \
  _Pragma("clang diagnostic ignored \"-Wnon-pod-varargs\"") \
  _Pragma("clang diagnostic ignored \"-Wdouble-promotion\"") \
  do {                                                               \
    const char *fmt;                                                 \
    NANOLOG_AUTOFORMAT(fmt, s, __VA_ARGS__)                          \
    nanolog::log(nanolog::kDEBUG, __FILE__, __func__, __LINE__, fmt, \
                 __VA_ARGS__);                                       \
  } while (0) \
  _Pragma("clang diagnostic pop")
#else
#define NANOLOG_DEBUG(s, ...)                                        \
  do {                                                               \
    const char *fmt;                                                 \
    NANOLOG_AUTOFORMAT(fmt, s, __VA_ARGS__)                          \
    nanolog::log(nanolog::kDEBUG, __FILE__, __func__, __LINE__, fmt, \
                 __VA_ARGS__);                                       \
  } while (0)
#endif

#ifdef __clang__
#define NANOLOG_INFO(s, ...)                              \
  _Pragma("clang diagnostic push") \
  _Pragma("clang diagnostic ignored \"-Wnon-pod-varargs\"") \
  _Pragma("clang diagnostic ignored \"-Wdouble-promotion\"") \
  do {                                                              \
    const char *fmt;                                                \
    NANOLOG_AUTOFORMAT(fmt, s, __VA_ARGS__)                         \
    nanolog::log(nanolog::kINFO, __FILE__, __func__, __LINE__, fmt, \
                 __VA_ARGS__);                                      \
  } while (0) \
  _Pragma("clang diagnostic pop")
#else
#define NANOLOG_INFO(s, ...)                                        \
  do {                                                              \
    const char *fmt;                                                \
    NANOLOG_AUTOFORMAT(fmt, s, __VA_ARGS__)                         \
    nanolog::log(nanolog::kINFO, __FILE__, __func__, __LINE__, fmt, \
                 __VA_ARGS__);                                      \
  } while (0)
#endif

#ifdef __clang__
#define NANOLOG_WARN(s, ...)                              \
  _Pragma("clang diagnostic push") \
  _Pragma("clang diagnostic ignored \"-Wnon-pod-varargs\"") \
  _Pragma("clang diagnostic ignored \"-Wdouble-promotion\"") \
  do {                                                              \
    const char *fmt;                                                \
    NANOLOG_AUTOFORMAT(fmt, s, __VA_ARGS__)                         \
    nanolog::log(nanolog::kWARN, __FILE__, __func__, __LINE__, fmt, \
                 __VA_ARGS__);                                      \
  } while (0) \
  _Pragma("clang diagnostic pop")
#else
#define NANOLOG_WARN(s, ...)                                        \
  do {                                                              \
    const char *fmt;                                                \
    NANOLOG_AUTOFORMAT(fmt, s, __VA_ARGS__)                         \
    nanolog::log(nanolog::kWARN, __FILE__, __func__, __LINE__, fmt, \
                 __VA_ARGS__);                                      \
  } while (0)
#endif

#ifdef __clang__
#define NANOLOG_ERROR(s, ...)                              \
  _Pragma("clang diagnostic push") \
  _Pragma("clang diagnostic ignored \"-Wnon-pod-varargs\"") \
  _Pragma("clang diagnostic ignored \"-Wdouble-promotion\"") \
  do {                                                               \
    const char *fmt;                                                 \
    NANOLOG_AUTOFORMAT(fmt, s, __VA_ARGS__)                          \
    nanolog::log(nanolog::kERROR, __FILE__, __func__, __LINE__, fmt, \
                 __VA_ARGS__);                                       \
  } while (0) \
  _Pragma("clang diagnostic pop")
#else
#define NANOLOG_ERROR(s, ...)                                        \
  do {                                                               \
    const char *fmt;                                                 \
    NANOLOG_AUTOFORMAT(fmt, s, __VA_ARGS__)                          \
    nanolog::log(nanolog::kERROR, __FILE__, __func__, __LINE__, fmt, \
                 __VA_ARGS__);                                       \
  } while (0)
#endif

#ifdef __clang__
#define NANOLOG_FATAL(s, ...)                              \
  _Pragma("clang diagnostic push") \
  _Pragma("clang diagnostic ignored \"-Wnon-pod-varargs\"") \
  _Pragma("clang diagnostic ignored \"-Wdouble-promotion\"") \
  do {                                                               \
    const char *fmt;                                                 \
    NANOLOG_AUTOFORMAT(fmt, s, __VA_ARGS__)                          \
    nanolog::log(nanolog::kFATAL, __FILE__, __func__, __LINE__, fmt, \
                 __VA_ARGS__);                                       \
  } while (0) \
  _Pragma("clang diagnostic pop")
#else
#define NANOLOG_FATAL(s, ...)                                        \
  do {                                                               \
    const char *fmt;                                                 \
    NANOLOG_AUTOFORMAT(fmt, s, __VA_ARGS__)                          \
    nanolog::log(nanolog::kFATAL, __FILE__, __func__, __LINE__, fmt, \
                 __VA_ARGS__);                                       \
  } while (0)
#endif

#elif defined(NANOLOG_USE_FMTLIB)

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

#else

// internal

class LogMsg
{
 public:
  LogMsg(enum loglevel level, const char *fname, const char *funname, int line)
    : _level(level), _fname(fname), _funname(funname), _line(line) {
  }

  void log() noexcept {
  }

  template<typename T, typename ... Args>
  void log(T head, Args ... rest) noexcept {
    std::stringstream ss;
    ss << head;
    arg_strs.push_back(ss.str());

    log(rest...);
  }

  void fmt(const char *msg) {

    _msg = msg;
  }

  void emit();

 private:
  LogMsg() = delete;
  LogMsg(const LogMsg &other) = delete;
  LogMsg &operator=(const LogMsg &other) = delete;

  enum loglevel _level;
  std::string _fname;
  std::string _funname;
  int _line;
  std::string _msg;

  std::vector<std::string> arg_strs;

};

#define NANOLOG_TRACE(s, ...)                                      \
  do {                                                             \
    nanolog::LogMsg msg(nanolog::kTRACE, __FILE__, __func__, __LINE__);     \
    msg.fmt(s); \
    msg.log(__VA_ARGS__); \
    msg.emit(); \
  } while (0)

#define NANOLOG_INFO(s, ...)                                      \
  do {                                                             \
    nanolog::LogMsg msg(nanolog::kINFO, __FILE__, __func__, __LINE__);     \
    msg.fmt(s); \
    msg.log(__VA_ARGS__); \
    msg.emit(); \
  } while (0)

#define NANOLOG_DEBUG(s, ...)                                      \
  do {                                                             \
    nanolog::LogMsg msg(nanolog::kDEBUG, __FILE__, __func__, __LINE__);     \
    msg.fmt(s); \
    msg.log(__VA_ARGS__); \
    msg.emit(); \
  } while (0)

#define NANOLOG_WARN(s, ...)                                      \
  do {                                                             \
    nanolog::LogMsg msg(nanolog::kWARN, __FILE__, __func__, __LINE__);     \
    msg.fmt(s); \
    msg.log(__VA_ARGS__); \
    msg.emit(); \
  } while (0)

#define NANOLOG_ERROR(s, ...)                                      \
  do {                                                             \
    nanolog::LogMsg msg(nanolog::kERROR, __FILE__, __func__, __LINE__);     \
    msg.fmt(s); \
    msg.log(__VA_ARGS__); \
    msg.emit(); \
  } while (0)

#define NANOLOG_FATAL(s, ...)                                      \
  do {                                                             \
    nanolog::LogMsg msg(nanolog::kFATAL, __FILE__, __func__, __LINE__);     \
    msg.fmt(s); \
    msg.log(__VA_ARGS__); \
    msg.emit(); \
  } while (0)

#endif


}  // namespace nanolog

#endif  // NANOLOG_HH_
