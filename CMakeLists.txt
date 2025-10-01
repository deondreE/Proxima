cmake_minimum_required(VERSION 3.15)
project(ProximaUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(PROXIMA_UI_PLATFORM_SOURCES "")
set(PROXIMA_UI_PLATFORM_LIBS "")
set(PROXIMA_UI_PLATFORM_INCLUDE_DIRS "")

if (APPLE)
  message(STATUS "Configuring for macOS (Metal Rendering)")

  find_library(METAL_FRAMEWORK Metal REQUIRED)
  find_library(METALTK_FRAMEWORK MetalKit REQUIRED)
  find_library(QUARTZ_FRAMEWORK QuartzCore REQUIRED)

  list(APPEND PROXIMA_UI_PLATFORM_LIBS
    ${METAL_FRAMEWORK}
    ${METALTK_FRAMEWORK}
    ${QUARTZ_FRAMEWORK}
  )

  set(PROXIMA_USE_SDL FALSE)
elseif(UNIX)
  message(STATUS "Configure for linux")
  find_package(SDL3 CONFIG REQUIRED)
  if (NOT TARGET SDL3::SDL3)
    message(FATAL_ERROR "SDL3 target not found. Ensure SDL3 is installed or check find_package paths.")
  endif()
  list(APPEND PROXIMA_UI_PLATFORM_LIBS SDL3::SDL3)

  add_subdirectory(external/sdl_image)

  find_package(SDL3_ttf CONFIG REQUIRED)
  if (NOT TARGET SDL3_ttf::SDL3_ttf)
    message(FATAL_ERROR "SDL3_ttf target not found. Ensure SDL3_ttf is installed or check find_package paths.")
  endif()
  list(APPEND PROXIMA_UI_PLATFORM_LIBS SDL3_ttf::SDL3_ttf)

  set(PROXIMA_USE_SDL TRUE)
elseif(WIN32)
  message(STATUS "Configuring Windows")

  list (APPEND PROXIMA_UI_PLATFORM_LIBS user32 gdi32 uuid ole32)

  set(PROXIMA_USE_SDL FALSE)
  set(PROXIMA_GUI_SUBSYSTEM "WIN32")

else()
  message(FATAL_ERROR "Windows Sucks")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# add_subdirectory(external/yaml-cpp)
  #src/Button.cpp
  #src/StackLayout.cpp
  #src/ConfigManager.cpp

  #src/Core/EventDispatcher.cpp
  #src/GridLayout.cpp
  #src/TextInput.cpp
  #src/Rect.cpp
  #src/Image.cpp
  #src/Line.cpp
  #src/Slider.cpp
  #src/Core/Debug.cpp
  #src/Core/DebugManager.cpp

add_library(proxima_ui SHARED
  src/View.cpp
  src/Core/WIN_Renderer.cpp
  src/W_Window.cpp
  src/Text.cpp
  src/View.cpp
)

target_include_directories(proxima_ui PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${PROXIMA_UI_PLATFORM_INCLUDE_DIRS}
)

target_link_libraries(proxima_ui PUBLIC
  ${PROXIMA_UI_PLATFORM_LIBS}
)

if (PROXIMA_USE_SDL)
  target_link_libraries(proxima_ui PUBLIC
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    SDL3_image::SDL3_image
  )
endif()

if (PROXIMA_USE_SDL)
  add_executable(example_app examples/unix/main.cpp)
  target_link_libraries(example_app PRIVATE
      yaml-cpp
      proxima_ui
      SDL3::SDL3
      SDL3_ttf::SDL3_ttf
      SDL3_image::SDL3_image
  )

elseif(WIN32)
  add_executable(example_app WIN32 examples/windows/main.cpp)
  target_compile_definitions(proxima_ui PRIVATE EXPORT)
  target_link_libraries(example_app PRIVATE proxima_ui)
endif()

if (APPLE)
  set_target_properties(exmaple_app PROPERTIES MACOSX_BUNDLE TRUE)
  set_target_properties(example_app APPEND_STRING PROPERTY COMPILE_OPTIONS "-xobjective-c++")
elseif(WIN32)
  set_target_properties(example_app PROPERTIES WIN32_SUBSYSTEM TRUE)
endif()
