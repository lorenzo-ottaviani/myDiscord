cmake_minimum_required(VERSION 3.15)
project(mydiscord C)

set(CMAKE_C_STANDARD 99)

# Define the compiler explicitly for Windows MSYS2
# Définir le compilateur explicitement pour Windows MSYS2
if (WIN32)
    set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
endif()

# Add compilation options for GCC
# Ajout d’options de compilation pour GCC
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

# === PostgreSQL libpq ===
if (WIN32)
    set(LIBPQ_INCLUDE_DIR "C:/Program Files/PostgreSQL/16/include")
    set(LIBPQ_LIBRARY_DIR "C:/Program Files/PostgreSQL/16/lib")
    include_directories(${LIBPQ_INCLUDE_DIR})
    set(LIBPQ_LIBRARIES pq)
else()  # Linux
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBPQ REQUIRED libpq)
    include_directories(${LIBPQ_INCLUDE_DIRS})
endif()

# === GTK4 ===
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)
include_directories(${GTK4_INCLUDE_DIRS})

# === CSS ===
file(COPY assets/myDiscord.css DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# # === CJSON ===
# pkg_check_modules(CJSON REQUIRED libcjson)
# include_directories(${CJSON_INCLUDE_DIRS})

# === Client ===
file(GLOB CLIENT_SOURCES "client/src/*.c")
add_executable(client ${CLIENT_SOURCES})
target_include_directories(client PRIVATE client/include)

# === Libsodium ===
if (WIN32)
    set(SODIUM_INCLUDE_DIR "C:/msys64/ucrt64/include")
    set(SODIUM_LIBRARY_DIR "C:/msys64/ucrt64/lib")
    include_directories(${SODIUM_INCLUDE_DIR})
    link_directories(${SODIUM_LIBRARY})

else()  # For Linux
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SODIUM REQUIRED libsodium)    
    include_directories(${SODIUM_INCLUDE_DIRS})
    link_directories(${SODIUM_LIBRARY_DIRS})    
endif()


# Link libraries conditionally based on the platform
if(WIN32)
    target_link_libraries(client PRIVATE ws2_32 ${LIBPQ_LIBRARIES} ${CJSON_LIBRARIES} ${GTK4_LIBRARIES} ${SODIUM_LIBRARY_DIRS})
else()
    target_link_libraries(client PRIVATE ${LIBPQ_LIBRARIES} ${CJSON_LIBRARIES} ${GTK4_LIBRARIES} ${SODIUM_LIBRARIES})
endif()

# Enable verbose output for compilation
# Activer la génération de fichiers détaillés pour la compilation
set(CMAKE_VERBOSE_MAKEFILE ON)