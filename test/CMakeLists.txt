cmake_minimum_required(VERSION 3.14)
project(OpCoSerializerTests)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(./../include)
include_directories(./../ThirdParty/include)

add_executable(opcoserializertests
    ./PropertiesTests.cpp
    ./JsonSerializerTests.cpp)

target_link_libraries(opcoserializertests gtest_main)

include(GoogleTest)
gtest_discover_tests(opcoserializertests)
