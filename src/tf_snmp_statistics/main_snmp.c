#include <stdio.h>
#include <string.h>
#include "../version.h"

#include "tf_snmp_module.h"
#include "utils/debug.h"
#include "configPSW/configPSW_main.h"
#include "statusPSW/statusPSW_main.h"
#include "statusPSW/poeStatus/poeStatus_main.h"

static void init_mib_tree(void);
static uint16_t init_mib_tree_main(void);

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

        print_node_info(next_node);
    }
    else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        return 1;
    }
    return 0;
}

static void init_mib_tree(void)
{
    uint16_t psw = init_mib_tree_main();
    uint16_t configPSW = init_mib_configPSW(psw);
    uint16_t statusPSW = init_mib_statusPSW(psw);
    uint16_t last_parent_indx = init_mib_tree_poeStatus(statusPSW);
}

static uint16_t init_mib_tree_main(void)
{
    LOG_DEV("Initializing MIB tree...\n");
    // Корневая ветка
    uint16_t root = add_node(1, "iso", NODE_INTERNAL, 0xFFFF, NULL, NULL);
    uint16_t org = add_node(3, "org", NODE_INTERNAL, root, NULL, NULL);
    uint16_t dod = add_node(6, "dod", NODE_INTERNAL, org, NULL, NULL);
    uint16_t internet = add_node(1, "internet", NODE_INTERNAL, dod, NULL, NULL);
    uint16_t private = add_node(4, "private", NODE_INTERNAL, internet, NULL, NULL);
    uint16_t enterprises = add_node(1, "enterprises", NODE_INTERNAL, private, NULL, NULL);
    uint16_t forttelecom = add_node(42019, "forttelecomMIB", NODE_INTERNAL, enterprises, NULL, NULL);

    uint16_t sw = add_node(3, "switch", NODE_INTERNAL, forttelecom, NULL, NULL);
    uint16_t psw = add_node(2, "psw", NODE_INTERNAL, sw, NULL, NULL);
    return psw;
}



