cmake_minimum_required(VERSION 3.0.0)
set (CMAKE_CXX_STANDARD 20)
project(trading-simulator)

include(CTest)
enable_testing()

add_subdirectory(src)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_subdirectory(tests)
add_subdirectory(third_party/trompeloeil)
