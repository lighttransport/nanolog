#include <iostream>
#include <utility>
#include <vector>
#include <sstream>

//#include <fmt/core.h>

void func(...) {
  printf("aaa\n");
}

#define BORA(s, ...) { \
  func(s, __VA_ARGS__); \
}

static std::vector<std::string> split_string(const std::string &input, const std::string &delimiter) {
  std::istringstream ss(input);
  std::string token;
  std::string::const_iterator it;

  std::vector<std::string> result;

  while(std::getline(ss, token, *(it = delimiter.begin()))) {
      while(*(++it)) ss.get();
      std::cout << token << " " << '\n';

      result.push_back(token);
  }

  return result;
}

class Msg
{
 public:
  Msg(int line) : _line(line) {
    std::cout << "line = " << _line << "\n";
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

    std::cout << "positions.size: " << positions.size() << "\n";

    std::vector<std::string> toks = split_string(_msg, "{}");
    std::cout << "ntoks = " << toks.size() << "\n";
    for (auto &tok : toks) {
      std::cout << "tok: " << tok << "\n";
    }

    for (auto &arg : arg_strs) {
      std::cout << "arg: " << arg << "\n";
    }

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

};

#define MYFUN(s, ...) do { \
  struct strprv { \
    static constexpr const char *str() { return s; }; \
  }; \
  Msg msg(__LINE__); msg.fmt(strprv::str()); \
  msg.log_arg(__VA_ARGS__); \
  msg.flush(); \
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

  return 0;
}
