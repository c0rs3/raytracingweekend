cmake_minimum_required(VERSION 3.0.0...3.13)
project(rtend VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(MSVC)
  # MSVC / Release
  set(CMAKE_CXX_FLAGS_RELEASE "/Ox /Ob2 /GL /fp:fast /arch:AVX2 /MD /EHs-c-")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/LTCG /INCREMENTAL:NO")
else()
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=haswell -funroll-loops -ffast-math -fexceptions -fno-rtti")
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-flto -Wl,--no-incremental")
  else()
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "")
  endif()
endif()

message("Compiler settings set.")

file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/headers/*.h")
file(GLOB_RECURSE BENCHTOOL_SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/benchtools/src/*.cpp")
file(GLOB_RECURSE BENCHTOOL_HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/benchtools/headerss/*.h")

message("GLOB done.")

add_executable(rtend
    "${CMAKE_SOURCE_DIR}/main.cpp"
    ${HEADER_FILES}
    ${BENCHTOOL_SOURCE_FILES}
    ${BENCHTOOL_HEADER_FILES}
)

target_include_directories(rtend PRIVATE
    "${CMAKE_SOURCE_DIR}/headers"
    "${CMAKE_SOURCE_DIR}/benchtools/headers"
)

message("Done")
