#
# Copyright (C) 2025 Procion ByProcion@gmail.com
#
# This file is part of WMM-toolkit.
#  This work is licensed under the terms of the MIT license.
#
#  This files is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#  See the LICENSE file for more details.
#
cmake_minimum_required(VERSION 3.16)

set(PLUGIN Vortex)
project(${PLUGIN} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_library(${PLUGIN} SHARED

  src/vortex.cpp
  src/vortex.h
  src/widgets.cpp
)

target_include_directories(${PLUGIN} PUBLIC
  ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PLUGIN} PRIVATE
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Core

)

set_target_properties(${PLUGIN} PROPERTIES
    PREFIX ""
)


set_target_properties(${PLUGIN} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src/plugins/${PLUGIN}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src/plugins/${PLUGIN}
)

target_compile_definitions(${PLUGIN} PRIVATE VORTEX_LIBRARY)
