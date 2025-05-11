cmake_minimum_required(VERSION 3.15)
project(mydiscord C)

set(CMAKE_C_STANDARD 99)

# Set the compiler for Windows MSYS2
if (WIN32)
    set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
endif()

# Add GCC-specific compilation options
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

# === PostgreSQL libpq ===
if (WIN32)
    set(LIBPQ_INCLUDE_DIR "C:/Program Files/PostgreSQL/16/include")
    set(LIBPQ_LIBRARY_DIR "C:/Program Files/PostgreSQL/16/lib")
    include_directories(${LIBPQ_INCLUDE_DIR})
    link_directories(${LIBPQ_LIBRARY_DIR})

else()  # For Linux
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBPQ REQUIRED libpq)
    include_directories(${LIBPQ_INCLUDE_DIRS})
    link_directories(${LIBPQ_LIBRARY_DIRS})
endif()    

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

# === GTK4 ===
if(WIN32)
    set(ENV{PKG_CONFIG_PATH} "C:/msys64/ucrt64/lib/pkgconfig;C:/msys64/ucrt64/share/pkgconfig")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK4 REQUIRED gtk4)

    include_directories(${GTK4_INCLUDE_DIRS})
    link_directories(${GTK4_LIBRARY_DIRS})
    add_definitions(${GTK4_CFLAGS_OTHER})
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK4 REQUIRED gtk4)
    include_directories(${GTK4_INCLUDE_DIRS})
    link_directories(${GTK4_LIBRARY_DIRS})
endif()



# === CSS ===
file(COPY client/src/myDiscord.css DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# === Client ===
file(GLOB CLIENT_SOURCES "client/src/*.c")
add_executable(client ${CLIENT_SOURCES})
target_include_directories(client PRIVATE client/include)

if(WIN32)
    target_link_libraries(client PRIVATE ws2_32 ${LIBPQ_LIBRARIES} ${GTK4_LIBRARIES})
    target_link_libraries(client PRIVATE ${GTK4_LDFLAGS})
else()
    target_link_libraries(client PRIVATE ${LIBPQ_LIBRARIES} ${GTK4_LIBRARIES})
endif()

# === Server ===
file(GLOB SERVER_SOURCES "server/src/*.c")
add_executable(server ${SERVER_SOURCES})
include_directories(server/include)

# Link PostgreSQL to the server executable
if(WIN32)
    target_link_libraries(server PRIVATE ws2_32 pq)

else()  # For Linux
    target_link_libraries(server PRIVATE ${LIBPQ_LIBRARIES})

endif()

# Link Libsodium to the server executable
target_link_libraries(server PRIVATE sodium)

# Enable detailed build output
set(CMAKE_VERBOSE_MAKEFILE ON)
