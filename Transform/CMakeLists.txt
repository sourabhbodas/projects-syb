cmake_minimum_required(VERSION 2.8)

set (PROJECT_NAME Transform)

SET(CMAKE_CXX_FLAGS "-std=c++0x -Wall -g -pg")

SET(${PROJECTNAME}_SOURCES
    src/main.cpp
)

SET(${PROJECTNAME}_HEADERS

)

add_executable(${PROJECT_NAME} ${${PROJECTNAME}_HEADERS} ${${PROJECTNAME}_SOURCES})

target_link_libraries (${PROJECT_NAME})

