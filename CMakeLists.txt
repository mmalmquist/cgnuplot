cmake_minimum_required(VERSION 3.0)
enable_language(C)
project(cgnuplot)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -lm -Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Og -ggdb -Werror -Wno-error=unused-result -Wno-error=unused-parameter -Wno-error=unused-variable -Wno-error=unused-function")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")

add_executable(
  cgptest
  test/testing.c
  src/cgpstack.c
  src/cgnuplot.c
  src/cgpfigure.c
  src/cgpentry.c
  src/cgppoint.c
)
