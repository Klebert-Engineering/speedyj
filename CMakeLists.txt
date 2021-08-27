cmake_minimum_required(VERSION 3.14.0)
enable_testing()

if (NOT DEFINED PROJECT_NAME)
  project(speedyj)
  set(NOT_SUBPROJECT ON)
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
if (NOT MSVC)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
  set(CMAKE_CXX_FLAGS -fPIC)
endif()

## SpeedyJ
####

add_library(speedyj SHARED
  src/speedyj.cpp
  include/speedyj/speedyj.hpp
  include/speedyj/speedyj-impl.hpp)

target_compile_features(speedyj
  PUBLIC
    cxx_std_17)

target_include_directories(speedyj
  PRIVATE
    "src"
  PUBLIC
    "include")

target_compile_definitions(speedyj
  PRIVATE
    SPEEDYJ_BUILD=1)

if (SPEEDYJ_QT)
  find_package(Qt5 COMPONENTS Core REQUIRED)

  target_sources(speedyj
    PRIVATE
      src/speedyj-qt.cpp
      include/speedyj/speedyj-qt.hpp)

  target_link_libraries(speedyj
    PRIVATE
      Qt5::Core)
endif()

target_compile_options(speedyj
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Werror -fvisibility=hidden>)

add_library(speedyj::speedyj ALIAS speedyj)

if (NOT_SUBPROJECT)
  add_subdirectory(test)
endif()
