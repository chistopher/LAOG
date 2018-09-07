
cmake_minimum_required(VERSION 3.2)
project(master-thesis)

set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_VERBOSE_MAKEFILE on)

add_compile_options(
        #-Wall
        #-mavx2
)


SET(HEADERS
        include/Graph.h
        include/Network.h)


set(SOURCE_FILES
        source/Graph.cpp
        source/Network.cpp)


set(EXECUTABLE_FILES
        scripts/testing.cpp
        scripts/test_Graph.cpp
        scripts/test_Network.cpp
        scripts/laogen.cpp
        scripts/link_triple.cpp
        scripts/benchmark.cpp
        )


include_directories(./include)
add_library(laog STATIC ${SOURCE_FILES} ${HEADERS})
# add_library(laog SHARED ${SOURCE_FILES} ${HEADERS})
foreach(file ${EXECUTABLE_FILES})
    get_filename_component(executable ${file} NAME_WE)
    add_executable(${executable} ${file})
    # target_link_libraries(${executable} laog -static -static-libgcc -static-libstdc++)
    target_link_libraries(${executable} laog)
endforeach()
