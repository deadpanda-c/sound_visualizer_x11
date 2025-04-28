cmake_minimum_required(VERSION 3.22)
project(SoundVisualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

include_directories(
    include/
)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(PkgConfig REQUIRED)
pkg_check_modules(PULSE REQUIRED libpulse)
pkg_check_modules(X11 REQUIRED x11)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PULSE_INCLUDE_DIRS}
        ${X11_INCLUDE_DIRS}
        src/
)

target_link_libraries(${PROJECT_NAME}
    ${PULSE_LIBRARIES}
    ${X11_LIBRARIES}
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wpedantic
)
