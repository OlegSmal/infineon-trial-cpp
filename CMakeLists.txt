cmake_minimum_required(VERSION 3.10)
project(TrialApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

include_directories(include)
add_executable(trial_app src/main.cpp src/ClientWindow.cpp src/ServerWindow.cpp)

target_link_libraries(trial_app PRIVATE Qt6::Widgets)