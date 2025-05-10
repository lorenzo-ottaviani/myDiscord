cmake_minimum_required(VERSION 3.15)
project(mydiscord C)

set(CMAKE_C_STANDARD 99)

# Définir le compilateur explicitement pour Windows MSYS2
if (WIN32)
    set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
endif()

# Ajout d’options de compilation pour GCC
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

# === PostgreSQL libpq ===
# Définir les chemins des en-têtes et bibliothèques PostgreSQL
set(LIBPQ_INCLUDE_DIR "C:/Program Files/PostgreSQL/16/include")
set(LIBPQ_LIBRARY "C:/Program Files/PostgreSQL/16/lib")

# Ajouter les répertoires d'inclusion et de liaison
include_directories(${LIBPQ_INCLUDE_DIR})
link_directories(${LIBPQ_LIBRARY})

# === Libsodium ===
# Définir les chemins des en-têtes et bibliothèques Libsodium
set(SODIUM_INCLUDE_DIR "C:/msys64/ucrt64/include")
set(SODIUM_LIBRARY_DIR "C:/msys64/ucrt64/lib")

# Ajouter les répertoires d'inclusion et de liaison
include_directories(${SODIUM_INCLUDE_DIR})
link_directories(${SODIUM_LIBRARY_DIR})

# === Server ===
file(GLOB SERVER_SOURCES "server/src/*.c")
add_executable(server ${SERVER_SOURCES})
include_directories(server/include)

# Lier PostgreSQL à l'exécutable server
target_link_libraries(server PRIVATE ws2_32 pq ${PQ_LIBRARY})

# Lier Libsodium à l'exécutable server
target_link_libraries(server PRIVATE sodium)

# Activer la génération de fichiers détaillés pour la compilation
set(CMAKE_VERBOSE_MAKEFILE ON)
