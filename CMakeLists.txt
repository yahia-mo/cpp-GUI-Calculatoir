cmake_minimum_required(VERSION 3.16)
project(Calculator)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)

find_package(wxWidgets REQUIRED COMPONENTS core base)


include(${wxWidgets_USE_FILE})

add_executable(calc main.cpp)

target_include_directories(calc PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(calc PRIVATE ${wxWidgets_LIBRARIES})