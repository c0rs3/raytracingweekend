cmake_minimum_required(VERSION 3.0.0...3.13)
project(rtend VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(rtend main.cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELEASE "/Ox /Ob2 /GL /fp:fast /arch:AVX2 /MD /EHs-c-")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/LTCG /INCREMENTAL:NO")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)