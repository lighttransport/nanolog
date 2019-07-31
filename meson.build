project('nanolog', 'cpp')

incdir = include_directories(['deps/fmt/include', 'include'])
executable('run_test', ['deps/fmt/src/format.cc', 'deps/fmt/src/posix.cc', 'src/nanolog.cc', 'test/main.cc'], include_directories : incdir)
