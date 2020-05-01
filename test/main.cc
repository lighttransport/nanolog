#include "nanolog.hh"

#include <thread>
#include <cstdio>

// Workaround for macro expansionin MSVC
// https://stackoverflow.com/questions/5134523/msvc-doesnt-expand-va-args-correctly
#define EXPAND(x) x
#define MY_ASSERT(cond, ...) \
{ \
  if (!(cond)) {           \
    EXPAND(NANOLOG_FATAL(__VA_ARGS__)); \
    abort(); \
  } \
}

static void logger1()
{
  int n = 100;

  for (int i = 0; i < n; i++) {
    NANOLOG_INFO("thread1 i = {}", i);
    std::this_thread::sleep_for(std::chrono::milliseconds(33));
  }
}

static void logger2()
{
  int n = 120;

  for (int i = 0; i < n; i++) {
    NANOLOG_INFO("thread2 i = {}", i);
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

  NANOLOG_TRACE("argc {}, argv {s}", argc, argv[0]);
  NANOLOG_DEBUG("argc {}, argv {s}", argc, argv[0]);
  NANOLOG_INFO("argc {}, argv {s}", argc, argv[0]);
  NANOLOG_WARN("argc {}, argv {s}", argc, argv[0]);
  NANOLOG_ERROR("argc {}, argv {s}", argc, argv[0]);
  //NANOLOG_FATAL("argc {}, argv {}", argc, argv[0]); // This may result in program abort()

  nanolog::set_color(false);
  NANOLOG_WARN("argc {}, argv {s}", argc, argv[0]);
  NANOLOG_ERROR("argc {}, argv {s}", argc, argv[0]);

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
