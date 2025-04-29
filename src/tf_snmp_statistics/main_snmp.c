#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../version.h"

#include "tf_snmp_module.h"
#include "utils.h"
#include "tf_snmp_module.h"
#include "poeStatus/poeStatus_main.h"

int main(const int argc, char **argv) {
    LOG_DEBUG("Start %s v%d.%d.%d", // Добавлен APP_VERSION_PATCH
              APP_NAME,
              APP_VERSION_MAJOR,
              APP_VERSION_MINOR,
              APP_VERSION_PATCH);

    if (argc != 3) {
        LOG_FATAL("Usage: %s [-n|-g|-s] OID\n", argv[0]);
        return 1;
    }

    char *action        = argv[1];
    char *input_oid_str = argv[2];

    LOG_DEBUG("input oid = %s action = %s", input_oid_str, action);
    int oid[MAX_OID_LEN];
    int oid_len = parse_oid(argv[2], oid);

    init_mib_tree();

    #if LOG_LEVEL < LOG_LEVEL_INFO
        LOG_DEBUG("Full MIB structure:\n");
        print_tree_debug(0, 0);
    #endif

    if(strcmp(argv[1], "-g") == 0) {
        uint16_t node = find_oid_node(oid, oid_len);
        //print_tree_debug(node, node);
        if(node == 0xFFFF) {
            LOG_INFO("OID not found\n");
            return 1;
        }
        char oid_buf[256];
        get_full_oid(node, oid_buf);
        LOG_INFO("current OID = %s", oid_buf);
        print_node_info(node);
    }
    else if(strcmp(argv[1], "-n") == 0) {
        LOG_DEV("GET NEXT REQUEST");
        uint16_t start_node = find_oid_node(oid, oid_len);
        if(start_node == 0xFFFF) start_node = find_min_subtree(0);
        uint16_t next_node = get_next_node(0, oid, 0, oid_len);
        if(next_node == 0xFFFF)
        {
            printf("End of MIB\n");
            return 1;
        }
        char oid_buf[256];
        get_full_oid(next_node, oid_buf);
    #if LOG_LEVEL < LOG_LEVEL_INFO
        print_tree_debug(next_node, next_node);
    #endif
        print_node_info(next_node);
    }
    else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        return 1;
    }
    return 0;
}

// LOG_DEBUG("Это отладка");   // Выводится, если LOG_LEVEL <= 0
// LOG_INFO("Информация");     // Выводится, если LOG_LEVEL <= 1
// LOG_WARN("Предупреждение"); // Выводится, если LOG_LEVEL <= 2
// LOG_ERROR("Ошибка");        // Выводится, если LOG_LEVEL <= 3
// LOG_FATAL("Критично");      // Выводится, если LOG_LEVEL <= 4