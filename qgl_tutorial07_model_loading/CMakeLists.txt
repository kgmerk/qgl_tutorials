cmake_minimum_required(VERSION 3.2.0)
project(qgl_tutorial07)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Widgets finds its own dependencies.
find_package(Qt5Widgets REQUIRED)

add_subdirectory(assimp-3.1.1)
set(ASSIMP_ROOT_DIR ${CMAKE_SOURCE_DIR}/assimp-3.1.1)
include_directories(${ASSIMP_ROOT_DIR}/include)
#link_directories(${ASSIMP_ROOT_DIR}/lib)

add_executable(qgl_tutorial07 qgl_tutorial07.cpp myresources.qrc)
target_link_libraries(qgl_tutorial07 assimp)

qt5_use_modules(qgl_tutorial07 Widgets)


