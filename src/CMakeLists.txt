cmake_minimum_required(VERSION 3.26)
project(tf_snmp C)

set(CMAKE_C_STANDARD 17)


#set(PROJECT_VERSION_MAJOR 3)
#set(PROJECT_VERSION_MINOR 1)
#set(PROJECT_VERSION_PATCH 0)
#set(PKG_RELEASE 543 CACHE STRING "Release number")

set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 2)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}-${PROJECT_VERSION_PATCH}")

message(">>>>>>>>>>> PKG_VERSION_MAJOR " ${PROJECT_VERSION_MAJOR})
message(">>>>>>>>>>> PKG_VERSION_MINOR " ${PROJECT_VERSION_MINOR})
message(">>>>>>>>>>> PROJECT_VERSION_PATCH " ${PROJECT_VERSION_PATCH})

message("CMAKE_CURRENT_SOURCE_DIR" ${CMAKE_CURRENT_SOURCE_DIR})
message("CMAKE_CURRENT_BINARY_DIR" ${CMAKE_CURRENT_BINARY_DIR})

configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
        "${CMAKE_CURRENT_SOURCE_DIR}/version.h"
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

option(DEBUG "Enable debug mode" ON)

message(">>>>>>>>>>> DEBUG " ${DEBUG})

if(DEBUG)
    message(STATUS "Debug mode enabled")
    add_compile_definitions(DEBUG)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Поиск необходимых библиотек Net-SNMP
set(CMAKE_EXE_LINKER_FLAGS "-lnetsnmp")

# Исходные файлы
set(SOURCES
    main_snmp.c
    utils.c
    utils.h
    tf_snmp_module.h
    version.h
)

# Создание исполняемого файла
add_executable(tf_snmp ${SOURCES})

# Подключение библиотек
target_include_directories(tf_snmp
    PRIVATE ${NETSNMP_INCLUDE_DIRS}
)

# Линковка библиотек
target_link_libraries(tf_snmp
    ${NETSNMP_LIBRARIES}
    jansson
)

add_custom_command(TARGET tf_snmp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Built version: ${PKG_VERSION}-${PKG_RELEASE}"
)

find_package(Git)
if(GIT_FOUND)
    # Получить хеш текущего коммита
    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_COMMIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else()
    set(GIT_COMMIT_HASH "unknown")
endif()

# Получить текущее время
string(TIMESTAMP TIMESTAMP)

#include(GetGitRevisionDescription)
#get_git_head_revision(GIT_REFSPEC GIT_COMMIT_HASH)
#configure_file(
#    "${CMAKE_CURRENT_SOURCE_DIR}/src/version.c.in"
#    "${CMAKE_CURRENT_BINARY_DIR}/version.c"
#)

INSTALL(TARGETS tf_snmp
        RUNTIME DESTINATION /usr/bin/
)
