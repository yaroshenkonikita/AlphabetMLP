cmake_minimum_required(VERSION 3.5)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)

# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
# set(BUILD_GTEST ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(forward_feed forward_feed.cc test.cc)

target_link_libraries(forward_feed PRIVATE Model gtest gtest_main)

add_executable(learn learn.cc test.cc)

target_link_libraries(learn PRIVATE Model gtest gtest_main)

add_executable(load_weights load_weight.cc test.cc)

target_link_libraries(load_weights PRIVATE Model gtest gtest_main)

add_executable(save_weights save_weight.cc test.cc)

target_link_libraries(save_weights PRIVATE Model gtest gtest_main)

add_executable(reader reader.cc test.cc)

target_link_libraries(reader PRIVATE Model gtest gtest_main)
