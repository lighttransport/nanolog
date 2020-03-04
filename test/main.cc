#include "nanolog.hh"

#include <thread>

void logger1()
{
  int n = 100;

  for (int i = 0; i < n; i++) {
    NANOLOG_INFO("thread1 i = {}", i);
    std::this_thread::sleep_for(std::chrono::milliseconds(33));
  }
}

void logger2()
{
  int n = 120;

  for (int i = 0; i < n; i++) {
    NANOLOG_INFO("thread2 i = {}", i);
    std::this_thread::sleep_for(std::chrono::milliseconds(27));
  }
}

int main(int argc, char **argv)
{
  nanolog::set_level(nanolog::kTRACE);
  nanolog::set_apptag("myapp");

  NANOLOG_INFO("start");

  NANOLOG_TRACE("argc {}, argv {s}", argc, argv[0]);
  NANOLOG_DEBUG("argc {}, argv {s}", argc, argv[0]);
  NANOLOG_INFO("argc {}, argv {s}", argc, argv[0]);
  NANOLOG_WARN("argc {}, argv {s}", argc, argv[0]);
  NANOLOG_ERROR("argc {}, argv {s}", argc, argv[0]);
  //NANOLOG_FATAL("argc {}, argv {}", argc, argv[0]);

  nanolog::set_color(false);
  NANOLOG_WARN("argc {}, argv {s}", argc, argv[0]);
  NANOLOG_ERROR("argc {}, argv {s}", argc, argv[0]);

  std::thread th1(logger1);
  std::thread th2(logger2);

  th1.join();
  th2.join();

  return 0;
}
