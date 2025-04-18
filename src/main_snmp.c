#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "version.h"

#include "tf_snmp_module.h"
#include "utils.h"

int main(int argc, char **argv) {

    LOG_DEBUG("Start %s v%d.%d.%d", // Добавлен APP_VERSION_PATCH
              APP_NAME,
              APP_VERSION_MAJOR,
              APP_VERSION_MINOR,
              APP_VERSION_PATCH);

    if (argc != 3) {
        print_usage(argv[0]);
        return 1;
    }

    char *action        = argv[1];
    char *input_oid_str = argv[2];

    LOG_INFO("input oid = %s action = %s", input_oid_str, action);

    LOG_DEBUG("Это отладка");   // Выводится, если LOG_LEVEL <= 0
    LOG_INFO("Информация");     // Выводится, если LOG_LEVEL <= 1
    LOG_WARN("Предупреждение"); // Выводится, если LOG_LEVEL <= 2
    LOG_ERROR("Ошибка");        // Выводится, если LOG_LEVEL <= 3
    LOG_FATAL("Критично");      // Выводится, если LOG_LEVEL <= 4


    return 0;
}

//LOG_WARN(">>>> LOG_WARN");
//LOG_ERROR(">>> LOG_ERROR");
//LOG_FATAL(">>> LOG_FATAL");