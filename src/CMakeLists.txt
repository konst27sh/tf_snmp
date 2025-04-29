cmake_minimum_required(VERSION 3.26)
project(tf_snmp C)

set(CMAKE_C_STANDARD 17)

set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 3)
set(PROJECT_VERSION_PATCH 6)
set(PROJECT_LOG_LEVEL 3)
set(DEBUG_ENABLE 1)

set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}-${PROJECT_VERSION_PATCH}")
set(PKG_VERSION_MAKE "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/version.h"
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(DEBUG_ENABLE EQUAL 1)
    message(STATUS "Debug mode enabled")
    add_compile_definitions(DEBUG LOG_LEVEL=${PROJECT_LOG_LEVEL})
    set(CMAKE_BUILD_TYPE Debug)
else()
    remove_definitions(-DDEBUG)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Поиск необходимых библиотек Net-SNMP
set(CMAKE_EXE_LINKER_FLAGS "-lnetsnmp")

# Исходные файлы
set(SOURCES
        tf_snmp_statistics/main_snmp.c
        tf_snmp_statistics/utils.c
        tf_snmp_statistics/utils.h
        tf_snmp_statistics/tf_snmp_module.h
    version.h
        tf_snmp_statistics/poeStatus/poeStatus_main.c
        tf_snmp_statistics/poeStatus/poeStatus_main.h
        tf_snmp_statistics/poeStatus/poe_handlers.c
        tf_snmp_statistics/poeStatus/poe_handlers.h
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
    COMMAND ${CMAKE_COMMAND} -E echo "Built version: ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}-${PROJECT_VERSION_PATCH}"
)

#add_executable(tf_snmp_test
#        snmp_main_test.c
#        tf_snmp_module.h
#        poeStatus/poeStatus_main.c
#        poeStatus/poeStatus_main.h
#        poeStatus/poe_handlers.c
#        poeStatus/poe_handlers.h
#        )
#

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

#INSTALL(TARGETS tf_snmp_test
#        RUNTIME DESTINATION /usr/bin/
#)
