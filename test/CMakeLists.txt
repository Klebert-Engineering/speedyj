project(speedyj-test)
unset(NOT_SUBPROJECT)

Include(FetchContent)

if (NOT TARGET Catch2)
  FetchContent_Declare(Catch2
    GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
    GIT_TAG        "v3.4.0"
    GIT_SHALLOW    On)
  FetchContent_MakeAvailable(Catch2)
endif()

if (TARGET Catch2)
  get_target_property(CATCH2_SOURCE_DIR Catch2 SOURCE_DIR)
  list(APPEND CMAKE_MODULE_PATH "${CATCH2_SOURCE_DIR}/contrib")
endif()

add_executable(speedyj-test
  stream.cpp)

target_compile_options(speedyj-test
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Werror>)

target_link_libraries(speedyj-test
  PUBLIC
    speedyj
    Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(speedyj-test)
