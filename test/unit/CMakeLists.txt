# https://google.github.io/googletest/quickstart-cmake.html
# テスト環境構築 ############################################################################################
cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# ft_irc関連のソースをgoogleTestに追加 ############################################################################################
file(GLOB_RECURSE LIB_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ../../src/*.cpp)

# main.cを除外
list(REMOVE_ITEM LIB_SRCS ../../src/main.cpp)

add_library(libftirc ${LIB_SRCS})

target_compile_definitions(libftirc PRIVATE UNIT_TEST)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}/../../include"
  # "${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/googletest-src/googletest/include"
)

# テストファイルのビルド ############################################################################################
file(GLOB_RECURSE TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ./src/*.cpp)

foreach(testSrc ${TEST_SRCS})
  # Extract the filename without an extension (NAME_WE)
  get_filename_component(testName ${testSrc} NAME_WE)

  # Add compile target
  add_executable(${testName} ${testSrc})

  # Link to GoogleTest
  target_link_libraries(${testName} GTest::gtest_main libftirc)

  # マクロ定義
  target_compile_definitions(${testName} PRIVATE UNIT_TEST)

  # Move testing binaries into a testBin directory
  set_target_properties(${testName} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

  # Add it to test execution
  add_test(NAME ${testName}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/bin/${testName})
endforeach(testSrc)
