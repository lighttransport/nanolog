cmake_minimum_required(VERSION 3.7)

set(TEST_TARGET "test_nanolog")

project(${TEST_TARGET})

# default: Use internal backend(fast to compile, limited log formatting)
option(NANOLOG_USE_FMTLIB "Use fmtlib(feature rich)" OFF)
option(NANOLOG_USE_PPRINTPP "Use pprintpp backend(faster to compile than fmtlib, but has some limitation in log formattring)" OFF)
option(NANOLOG_ANDROID_USE_STDIO "Print logs to stdout(For Android)" OFF)

set(CMAKE_CXX_STANDARD 11)

find_package(Threads REQUIRED)

set(NANOLOG_TEST_SOURCES
    test/main.cc
    src/nanolog.cc
    )

if (NANOLOG_USE_FMTLIB)

  list(APPEND NANOLOG_TEST_SOURCES ${PROJECT_SOURCE_DIR}/deps/fmt/src/format.cc)

  if (NOT ANDROID)
    list(APPEND NANOLOG_TEST_SOURCES
      ${PROJECT_SOURCE_DIR}/deps/fmt/src/os.cc
      )
  endif (NOT ANDROID)

endif (NANOLOG_USE_FMTLIB)

add_executable(${TEST_TARGET}
               ${NANOLOG_TEST_SOURCES}
              )

target_link_libraries(${TEST_TARGET} PUBLIC Threads::Threads)

if (NANOLOG_USE_FMTLIB)
  target_compile_definitions(${TEST_TARGET} PRIVATE NANOLOG_USE_FMTLIB)
  target_include_directories(${TEST_TARGET} PRIVATE ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/deps/fmt/include)
elseif (NANOLOG_USE_PPRINTPP)
  target_compile_definitions(${TEST_TARGET} PRIVATE NANOLOG_USE_PPRINTPP)
  target_include_directories(${TEST_TARGET} PRIVATE ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/deps/pprintpp/include)
else ()
  target_include_directories(${TEST_TARGET} PRIVATE ${PROJECT_SOURCE_DIR}/include
    )
endif (NANOLOG_USE_FMTLIB)

if (ANDROID)
  if (NANOLOG_ANDROID_USE_STDIO)
    target_compile_definitions(${TEST_TARGET} PRIVATE NANOLOG_ANDROID_USE_STDIO)
  else (NANOLOG_ANDROID_USE_STDIO)
    target_link_libraries(${TEST_TARGET} PUBLIC log)
  endif (NANOLOG_ANDROID_USE_STDIO)
endif (ANDROID)
