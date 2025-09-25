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

  find_package(SDL3_ttf CONFIG REQUIRED)
  if (NOT TARGET SDL3_ttf::SDL3_ttf)
    message(FATAL_ERROR "SDL3_ttf target not found. Ensure SDL3_ttf is installed or check find_package paths.")
  endif()
  list(APPEND PROXIMA_UI_PLATFORM_LIBS SDL3_ttf::SDL3_ttf)

  set(PROXIMA_USE_SDL TRUE)
elseif(WIN32)
  message(STATUS "Configuring Windows")

  find_path(DIRECT_X_INCLUDE_DIR d3d11.h
      PATHS ENV DXSDK_DIR/Include
      NO_CMAKE_FIND_ROOT_PATH)
  find_library(D3D11_Library Names d3d11
      PATHS ENV DXSDK_DIR/Lib/x64 ENV DXDSK/Lib/x86
      NO_CMAKE_FIND_ROOT_PATH)
  find_library(DXGI_Library Names dxgi
      PATHS ENV DXSDK_Dir/Lib/x64 ENV DXSDK/Lib/x86
      NO_CMAKE_FIND_ROOT_PATH)
  
  if (NOT DIRECT_X_INCLUDE_DIR OR NOT D3D11_Library OR NOT DXGI_Library)
    message(FATAL_ERROR "DirectX 11 SDK not found. Please make it work.")
  endif()

  list(APPEND PROXIMA_UI_PLATFORM_INCLUDE_DIRS ${DIRECTX_INCLUDE_DIR})
  list(APPEND PROXIMA_UI_PLATFORM_LIBS ${D3D11_LIBRARY} ${DXGI_LIBRARY})

  set(PROXIMA_USE_SDL FALSE)
  set(PROXIMA_GUI_SUBSYSTEM "WIN32")

else() 
  message(FATAL_ERROR "Windows Sucks")
endif()



include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

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
  ${PROXIMA_UI_PLATFORM_INCLUDE_DIRS}
)

target_link_libraries(proxima_ui PUBLIC
  yaml-cpp
  ${PROXIMA_UI_PLATFORM_LIBS}
)

if (PROXIMA_USE_SDL)
  target_link_libraries(proxima_ui PUBLIC
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    SDL3_image::SDL3_image
  )
endif()


add_executable(example_app examples/main.cpp)

target_link_libraries(example_app PRIVATE 
    yaml-cpp 
    proxima_ui
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    SDL3_image::SDL3_image
)

if (APPLE) 
  set_target_properties(exmaple_app PROPERTIES MACOSX_BUNDLE TRUE)
  set_target_properties(example_app APPEND_STRING PROPERTY COMPILE_OPTIONS "-xobjective-c++")
elseif(WIN32) 
  set_target_properties(example_app PROPERTIES WIN32_SUBSYSTEM TRUE)
endif()