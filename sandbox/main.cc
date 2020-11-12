#include <iostream>
#include <utility>
#include <vector>
#include <sstream>
#include <map>
#include <tuple>
#include <mutex>
#include <thread>
#include <chrono>

//#include <fmt/core.h>

void func(...) {
  printf("aaa\n");
}

#define BORA(s, ...) { \
  func(s, __VA_ARGS__); \
}

std::mutex mtx;

// key = <fname, funname, line>
std::map<std::tuple<std::string, std::string, int>, int64_t> g_countmap;

static std::vector<std::string> split_string(const std::string &input, const std::string &delimiter) {
  std::istringstream ss(input);
  std::string token;
  std::string::const_iterator it;

  std::vector<std::string> result;

  while(std::getline(ss, token, *(it = delimiter.begin()))) {
      while(*(++it)) ss.get();
      //std::cout << token << " " << '\n';

      result.push_back(token);
  }

  return result;
}

class Msg
{
 public:
  Msg(const char *fname, const char *funname, int line, int64_t max_count_ = -1) : _fname(fname), _funname(funname), _line(line), max_count(max_count_) {
    //std::cout << "line = " << _line << "\n";
  }

  void log_arg() noexcept {
  }

  template<typename T, typename ... Args>
  void log_arg(T head, Args ... rest) noexcept {
    std::stringstream ss;
    ss << head;
    arg_strs.push_back(ss.str());

    log_arg(rest...);
  }

  void fmt(const char *msg) {

    _msg = msg;
  }

  void flush() {

    // global lock
    std::lock_guard<std::mutex> lock(mtx);

    std::string str(_msg);

    // find the number of occurences of `{}`
    // https://stackoverflow.com/questions/4034750/find-all-a-substrings-occurrences-and-locations
    std::vector<size_t> positions;
    size_t pos = str.find("{}", 0);
    while(pos != std::string::npos)
    {
        positions.push_back(pos);
        pos = str.find("{}" ,pos+1);
    }

    //std::cout << "positions.size: " << positions.size() << "\n";

    std::vector<std::string> toks = split_string(_msg, "{}");
    //std::cout << "ntoks = " << toks.size() << "\n";
    //for (auto &tok : toks) {
    //  std::cout << "tok: " << tok << "\n";
    //}

    //for (auto &arg : arg_strs) {
    //  std::cout << "arg: " << arg << "\n";
    //}

    if (positions.size() != arg_strs.size()) {
      std::cerr << "argument mismatch!" << "\n";
      // Argument mismatch
      return;
    }

    // replace string from the end to the beginning so that we don't need to adjust
    // the location in `positions`.
    size_t n = positions.size();
    for (size_t i = 0; i < n; i++) {
      str.replace(positions[n - i - 1], /* strlen("{}") */2, arg_strs[n - i - 1]);
    }

    std::cout << str << "\n";

  }

  std::string _fname;
  std::string _funname;
  int _line;
  std::string _msg;

  Msg(Msg &&other) {
    std::cout << "rval ctor\n";
  }

 private:
  Msg() = delete;
  Msg(const Msg &other) = delete;
  Msg &operator=(const Msg &other) = delete;

  std::vector<std::string> arg_strs;

  int64_t count{0};
  int64_t max_count{-1};
};

#define MYFUN(s, ...) do { \
  struct strprv { \
    static constexpr const char *str() { return s; }; \
  }; \
  Msg msg(__FILE__, __func__, __LINE__); msg.fmt(strprv::str()); \
  msg.log_arg(__VA_ARGS__); \
  msg.flush(); \
} while(0)

// Only print the log `n` times.
#define MYFUN_N(n, s, ...) do { \
  bool reached{false}; \
  static int64_t counter = 0; \
  static std::mutex _mtx; \
  _mtx.lock(); \
  counter++; \
  if (counter > n) { reached = true; } \
  _mtx.unlock(); \
  struct strprv { \
    static constexpr const char *str() { return s; }; \
  }; \
  if (!reached) { \
    Msg msg(__FILE__, __func__, __LINE__); msg.fmt(strprv::str()); \
    msg.log_arg(__VA_ARGS__); \
    msg.flush(); \
  } \
} while(0)

// Do not prit log within `msec` milliseconds after the last time of this log was printed.
#define MYFUN_MS(msecs, s, ...) do { \
  bool suppressed{false}; \
  static std::chrono::time_point<std::chrono::system_clock> last_time(std::chrono::system_clock::from_time_t({})); \
  static std::mutex _mtx; \
  _mtx.lock(); \
  auto curr = std::chrono::system_clock::now(); \
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(curr - last_time).count(); \
  if (elapsed < msecs) { suppressed = true ; } else { last_time = curr; } \
  _mtx.unlock(); \
  struct strprv { \
    static constexpr const char *str() { return s; }; \
  }; \
  if (!suppressed) { \
    Msg msg(__FILE__, __func__, __LINE__); msg.fmt(strprv::str()); \
    msg.log_arg(__VA_ARGS__); \
    msg.flush(); \
  } \
} while(0)

//inline int log_all() noexcept {
//  std::cout << "hello\n";
//    return 0;
//}
//
//template <typename T>
//int log(T target) noexcept {
//  std::cout << "log: " << target << "\n";
//
//  return 0;
//}
//
//template <typename T, typename ... Args>
//inline void log_all(T target, Args ... rest) noexcept {
//    log(target);
//    log_all(rest...);
//}
//

void print_n(int id) {
  MYFUN_N(3, "print_n(3): {} th id", id);
}

void print_msec(int wait) {
  std::this_thread::sleep_for(std::chrono::milliseconds(wait));
  MYFUN_MS(1000, "print_n(3): wait {}", wait);
}

int main(int argc, char **argv)
{
  //log_all(1, 0.2, "foo", true);
  //std::string s = fmt::format("{}", argc);

  //std::cout << s << "\n";
  {
    struct strprv {
      static constexpr const char *str() { return "muda"; };
    };
  }

  MYFUN("{}aa");
  MYFUN("aa{}", 1);
  MYFUN("{} aa", 1);
  MYFUN("aa{}, {} bora", 3, "gogo");

  for (int i = 0; i < 10; i++) {
    //MYFUN_N(3, "this log is printed only three times");
  }

  {
    std::thread threads[10];
    // spawn 10 threads:
    for (int i=0; i<10; ++i) {
      threads[i] = std::thread(print_n,i+1);
    }
    for (auto& th : threads) th.join();
  }

  // spawn 1000 threads:
  {
    std::thread threads[1000];
    for (int i=0; i<1000; ++i) {
      threads[i] = std::thread(print_msec, (i+1) * 10);
    }
    for (auto& th : threads) th.join();
  }

  return 0;
}
