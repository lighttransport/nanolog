#include <iostream>
#include <utility>

//#include <fmt/core.h>

void func(...) {
  printf("aaa\n");
}

#define BORA(s, ...) { \
  func(s, __VA_ARGS__); \
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
    std::cout << "arg = " << head << "\n";
    //log_arg(head);
    log_arg(rest...);
  }

  Msg &&fmt(const char *msg) {

    std::cout << "&&fmt\n";
    _msg = msg;

    return std::forward<Msg>(*this);
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

};

#define MYFUN(s, ...) do { \
  struct strprv { \
    static constexpr const char *str() { return s; }; \
  }; \
  Msg msg(__LINE__); msg.fmt(strprv::str()).log_arg(__VA_ARGS__); \
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

  MYFUN("aa");
  MYFUN("aa", 1);
  MYFUN("aa", 3, 4);

  return 0;
}
