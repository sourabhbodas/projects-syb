cmake_minimum_required(VERSION 2.8)

set (PROJECT_NAME CirCleGridDetection)

find_package(OpenCV REQUIRED)

SET(CMAKE_CXX_FLAGS "-Wall -g -pg")

SET(${PROJECTNAME}_SOURCES
    src/main.cpp
    src/processingClass.cpp
)

SET(${PROJECTNAME}_HEADERS
    src/all_includes.h
    src/processingClass.h
)

add_executable(${PROJECT_NAME} ${${PROJECTNAME}_HEADERS} ${${PROJECTNAME}_SOURCES})

target_link_libraries (${PROJECT_NAME} ${OpenCV_LIBS} )

