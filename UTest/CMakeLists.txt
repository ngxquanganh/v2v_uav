cmake_minimum_required(VERSION 3.18.4)

project(anhnq)

find_package(GTest REQUIRED)

set(SOURCES src/UT_frameMess.cpp
            src/UT_linkedList.cpp  
            src/UT_handleMess.cpp
            src/UT_write.cpp
            )
set(SOURCES_LIB  
    ../src/linkedList.c
    ../src/frameMess.c)
    
# SET(CMAKE_CXX_FLAGS " -fsanitize=address -static-libasan -g -Wno-write-strings ")     
# SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage") 
# SET(GCC_COVERAGE_LINK_FLAGS "-lgcov") 
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -g -Wno-write-strings") 
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

add_library(lib STATIC ${SOURCES_LIB})
include_directories(../include)

add_executable(main ${SOURCES})
target_compile_definitions(main PRIVATE DEBUG)

target_link_libraries(main ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY} pthread lib)