
cmake_minimum_required(VERSION 3.3)
project(master-thesis)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(
        -Wall
        -mavx2
        -mfma
)


SET(HEADERS
        include/Graph.h
        include/Network.h)


set(SOURCE_FILES
        source/Graph.cpp
        source/Network.cpp)


set(EXECUTABLE_FILES
        scripts/testing.cpp
        scripts/Graph_test.cpp
        scripts/sumDistBenchmark.cpp
        scripts/laogen.cpp
        )


include_directories(./include)
add_library(laog STATIC ${SOURCE_FILES} ${HEADERS})
foreach(file ${EXECUTABLE_FILES})
    get_filename_component(executable ${file} NAME_WE)
    add_executable(${executable} ${file})
    target_compile_options(${executable} PRIVATE -O3)
    target_link_libraries(${executable} laog)
endforeach()
