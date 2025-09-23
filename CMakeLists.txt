cmake_minimum_required(VERSION 3.15)
project(ProximaUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
find_package(X11 REQUIRED)

add_library(proxima_ui
  src/View.cpp
  src/Text.cpp
  src/Button.cpp
  src/StackLayout.cpp
)

target_include_directories(proxima_ui PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(proxima_ui PUBLIC X11::X11)

add_executable(example_app examples/main.cpp)

target_link_libraries(example_app PRIVATE proxima_ui)
