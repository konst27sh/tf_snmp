cmake_minimum_required(VERSION 3.26)
project(tf_snmp C)

set(CMAKE_C_STANDARD 17)

# Поиск необходимых библиотек Net-SNMP
set(CMAKE_EXE_LINKER_FLAGS "-lnetsnmp")

# Исходные файлы
set(SOURCES
        main_snmp.c
        poe_module.c
        poe_module.h
        snmp_module.h
        snmp_utils.c
        snmp_utils.h
)

# Создание исполняемого файла
add_executable(tf_snmp ${SOURCES})

# Подключение библиотек
target_include_directories(tf_snmp PRIVATE ${NETSNMP_INCLUDE_DIRS})

# Линковка библиотек
target_link_libraries(tf_snmp
        ${NETSNMP_LIBRARIES}
        jansson
)

INSTALL(TARGETS tf_snmp
        RUNTIME DESTINATION /usr/bin/
)
