#include "nanolog.hh"

#include <thread>
#include <cstdio>
#include <iostream>

// Workaround for macro expansionin MSVC
// https://stackoverflow.com/questions/5134523/msvc-doesnt-expand-va-args-correctly
#define EXPAND(x) x
#define MY_ASSERT(cond, ...) \
do { \
  if (!(cond)) {           \
    EXPAND(NANOLOG_FATAL(__VA_ARGS__)); \
    abort(); \
  } \
} while(0)

struct vec3
{
  float x, y, z;
};

std::ostream &operator<<(std::ostream& ofs, const vec3 &v) {
  ofs << "(" << v.x << ", " << v.y << ", " << v.z << ")";

  return ofs;
}

static void logger1()
{
  int n = 100;

  for (int i = 0; i < n; i++) {
    NANOLOG_TRACE_N(10, "This log will be printed up to 10 times. i = {}", i);
    //NANOLOG_INFO("thread1 i = {}", i);
#if !defined(NANOLOG_USE_PPRINTPP)
    // print log each 1000 msecs(1 second)
    NANOLOG_ERROR_MSEC(1000, "thread1 i = {}", i);
#endif
    std::this_thread::sleep_for(std::chrono::milliseconds(33));
  }
}

static void logger2()
{
  int n = 120;

  for (int i = 0; i < n; i++) {
#if !defined(NANOLOG_USE_PPRINTPP)
    // print log each 2000 msecs(2 second)
    NANOLOG_INFO_MSEC(2000, "thread2 i = {}", i);
#else
    NANOLOG_INFO("thread2 i = {}", i);
#endif
    std::this_thread::sleep_for(std::chrono::milliseconds(27));
  }
}

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  nanolog::set_level(nanolog::kTRACE);
  nanolog::set_apptag("myapp");

  MY_ASSERT(argc < 16, "argc must be less than 16, but got {}", "muda");

  NANOLOG_INFO("{}", "start");
  std::string hello;
  NANOLOG_INFO("{}", hello.c_str());

  float x = 3.13f;
  NANOLOG_INFO("{}", x);

#if defined(NANOLOG_USE_FMTLIB)
  NANOLOG_TRACE("argc {}, argv {}", argc, argv[0]);
  NANOLOG_DEBUG("argc {}, argv {}", argc, argv[0]);
  NANOLOG_INFO("argc {}, argv {}", argc, argv[0]);
  NANOLOG_WARN("argc {}, argv {}", argc, argv[0]);
  NANOLOG_ERROR("argc {}, argv {}", argc, argv[0]);
  //NANOLOG_FATAL("argc {}, argv {}", argc, argv[0]); // This may result in program abort()

  nanolog::set_color(false);
  NANOLOG_WARN("argc {}, argv {}", argc, argv[0]);
  NANOLOG_ERROR("argc {}, argv {}", argc, argv[0]);
#elif defined(NANOLOG_USE_PPRINTPP)
  NANOLOG_TRACE("argc {}, argv {s}", argc, argv[0]);
  NANOLOG_DEBUG("argc {}, argv {s}", argc, argv[0]);
  NANOLOG_INFO("argc {}, argv {s}", argc, argv[0]);
  NANOLOG_WARN("argc {}, argv {s}", argc, argv[0]);
  NANOLOG_ERROR("argc {}, argv {s}", argc, argv[0]);
  //NANOLOG_FATAL("argc {}, argv {}", argc, argv[0]); // This may result in program abort()

  nanolog::set_color(false);
  NANOLOG_WARN("argc {}, argv {s}", argc, argv[0]);
  NANOLOG_ERROR("argc {}, argv {s}", argc, argv[0]);
#else
  NANOLOG_TRACE("argc {}, argv {}", argc, argv[0]);
  NANOLOG_DEBUG("argc {}, argv {}", argc, argv[0]);
  NANOLOG_INFO("argc {}, argv {}", argc, argv[0]);
  NANOLOG_WARN("argc {}, argv {}", argc, argv[0]);
  NANOLOG_ERROR("argc {}, argv {}", argc, argv[0]);
  //NANOLOG_FATAL("argc {}, argv {}", argc, argv[0]); // This may result in program abort()

  nanolog::set_color(false);
  NANOLOG_WARN("argc {}, argv {}", argc, argv[0]);
  NANOLOG_ERROR("argc {}, argv {}", argc, argv[0]);

  vec3 myvec;
  myvec.x = 1.2f;
  myvec.y = 1.5f;
  myvec.z = 2.4f;
  // Use `operator<<` to format vec3 value.
  NANOLOG_INFO("vec {}", myvec);

  // internal can use `std::string` argument
  NANOLOG_INFO("hello: {}", hello);

  // zero-arg log is supported.
  NANOLOG_INFO("world");
#endif

  {
    std::thread th1(logger1);
    std::thread th2(logger2);

    th1.join();
    th2.join();
  }

  nanolog::set_printtime(false);
  {
    std::thread th1(logger1);
    std::thread th2(logger2);

    th1.join();
    th2.join();
  }

  return 0;
}
