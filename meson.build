project('nanolog', 'cpp', default_options : ['cpp_std=c++11'])

thread_dep = dependency('threads')

incdir = include_directories(['deps/pprintpp/include', 'include'])

executable('run_test', ['src/nanolog.cc', 'test/main.cc'], include_directories : incdir, dependencies : thread_dep)
