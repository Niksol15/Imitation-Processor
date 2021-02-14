cmake_minimum_required(VERSION 3.17)
project(Lab3)

set(CMAKE_CXX_STANDARD 17)

list(APPEND HEADERS  Register.h Processor.h)

list(APPEND SOURCES main.cpp Register.cpp Processor.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})