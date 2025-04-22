cmake_minimum_required(VERSION 3.10)
project(myDiscord C)

set(CMAKE_C_STANDARD 11)

# Utilise pkg-config pour trouver GTK4
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

# Inclure les headers GTK4
include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})
add_definitions(${GTK4_CFLAGS_OTHER})

# ===  ===
file(GLOB SOURCES "src/*.c")
add_executable( ${SOURCES})
include_directories(include)
target_link_libraries( PRIVATE ${GTK4_LIBRARIES})