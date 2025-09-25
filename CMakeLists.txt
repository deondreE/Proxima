cmake_minimum_required(VERSION 3.15)
project(ProximaUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 CONFIG REQUIRED)
if (NOT TARGET SDL3::SDL3)
  message(FATAL_ERROR "SDL3 target not found. Ensure SDL3 is installed or check find_package paths.")
endif()

find_package(SDL3_ttf CONFIG REQUIRED)
if (NOT TARGET SDL3_ttf::SDL3_ttf)
  message(FATAL_ERROR "SDL3_ttf target not found. Ensure SDL3_ttf is installed or check find_package paths.")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
find_package(X11 REQUIRED)

add_subdirectory(external/yaml-cpp)
add_subdirectory(external/sdl_image)

add_library(proxima_ui
  src/View.cpp
  src/Button.cpp
  src/StackLayout.cpp
  src/ConfigManager.cpp
  src/Text.cpp
  src/Core/EventDispatcher.cpp
  src/TextInput.cpp
  src/Rect.cpp
  src/Image.cpp
  src/Line.cpp
  src/Slider.cpp
  src/Window.cpp
)

target_include_directories(proxima_ui PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(proxima_ui PUBLIC
  X11::X11
  yaml-cpp
  SDL3::SDL3
  SDL3_ttf::SDL3_ttf
  SDL3_image::SDL3_image
 )

add_executable(example_app examples/main.cpp)

target_link_libraries(example_app PRIVATE 
    yaml-cpp 
    proxima_ui
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    SDL3_image::SDL3_image
)
