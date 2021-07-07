project(speedyj-test)
unset(NOT_SUBPROJECT)

Include(FetchContent)

if (NOT TARGET Catch2)
  FetchContent_Declare(Catch2
    GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
    GIT_TAG        "v2.13.6"
    GIT_SHALLOW    On)
  FetchContent_MakeAvailable(Catch2)
  list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)
endif()

add_executable(speedyj-test
  main.cpp
  stream.cpp)

target_compile_options(speedyj-test
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Werror>)

target_link_libraries(speedyj-test
  PUBLIC
    speedyj
    Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(speedyj-test)
