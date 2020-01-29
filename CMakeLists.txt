cmake_minimum_required(VERSION 3.7)

set(TEST_TARGET "test_nanolog")

project(${TEST_TARGET})

option(NANOLOG_ANDROID_USE_STDIO "Print logs to stdout(For Android)" OFF)

set(CMAKE_CXX_STANDARD 11)

set(NANOLOG_TEST_SOURCES
    test/main.cc
    src/nanolog.cc
    ${PROJECT_SOURCE_DIR}/deps/fmt/src/format.cc
    )

if (NOT ANDROID)
  list(APPEND NANOLOG_TEST_SOURCES
    ${PROJECT_SOURCE_DIR}/deps/fmt/src/os.cc
    )
endif (NOT ANDROID)

add_executable(${TEST_TARGET}
               ${NANOLOG_TEST_SOURCES}
              )

if (NANOLOG_ANDROID_USE_STDIO)
  target_compile_definitions(${TEST_TARGET} PRIVATE NANOLOG_ANDROID_USE_STDIO)
endif (NANOLOG_ANDROID_USE_STDIO)

target_include_directories(${TEST_TARGET} PRIVATE ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/deps/fmt/include
  )
