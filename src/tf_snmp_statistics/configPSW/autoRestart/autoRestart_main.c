//
// Created by sheverdin on 4/30/25.
//

#include "string.h"
#include "autoRestart_main.h"
#include "../../utils/debug.h"
#include "../../utils/utils.h"

#define AR_MODE (4)

static int get_arConfig(uint16_t port, char* option, char *res);

const char *ar_columns[AUTO_RESTART_COL] = {
    "autoRstIndex",
    "autoRstMode",
    "autoRstDstIP",
    "autoRstSpeedDown",
    "autoRstSpeedUp",
    "autoReStartTimeOnHour",
    "autoReStartTimeOnMin",
    "autoReStartTimeOffHour",
    "autoReStartTimeOffMin",
};

typedef enum
{
    disabled    = 0,
    link        = 1,
    ping        = 2,
    speed       = 3,
    time        = 4,
}AR_MODE_e;

const char *ar_mode[AR_MODE] =
{
    "disabled",
    "link",
    "ping",
    "speed",
};

void get_autoRstIndex(StaticTreeNode *node)
{
    #if (LOG_LEVEL <= LOG_LEVEL_INFO)
        LOG_FATAL("get_autoRstIndex Port = %d", node->oid_component);
    #else
        printf("%d\n", node->oid_component);
    #endif
}

void get_autoRstMode(StaticTreeNode *node)
{
    char data[256];
    get_arConfig(node->oid_component, "mode", data);

    if (strlen(data) == 0)
    {
        strcpy(data, "NULL");
    }
    json_t *root = NULL;
    const char *status_v = NULL;
    AR_MODE_e arMode = disabled;
    root = getData_formJson(data);
    if (root != NULL)
    {
        json_t *value = json_object_get(root, "value");
        if (value != NULL)
        {
            status_v = json_string_value(value);

            for (AR_MODE_e i = 0; i < AR_MODE; i++)
            {
                if (strcmp(status_v, ar_mode[i]) == 0)
                {
                    arMode = i;
                    break;
                }
            }
        }
    }

    #if (LOG_LEVEL < LOG_LEVEL_INFO)
        LOG_FATAL("get_autoRstMode Port = %d", node->oid_component);
            LOG_FATAL("DATA: \n --  strlen = %lu\n", strlen(data));
            LOG_FATAL("%s ", data);
            LOG_FATAL("status_v = %s", status_v);
            LOG_FATAL("arMode = %d", arMode);

    #else
        printf("%d\n", arMode);
    #endif
}

void get_autoRstDstIP(StaticTreeNode *node)
{
    char data[256];
    get_arConfig(node->oid_component, "host", data);

    if (strlen(data) == 0)
    {
        strcpy(data, "0.0.0.0");
    }

    #if (LOG_LEVEL <= LOG_LEVEL_INFO)
        LOG_FATAL("get_autoRstDstIP Port = %d", node->oid_component);
        LOG_FATAL("%s", data);
        LOG_FATAL("DATA\n --  strlen = %lu\n", strlen(data));
    #else
        printf("%s\n", data);
    #endif
}

void get_autoRstSpeedDown(StaticTreeNode *node)
{
    char data[256];
    get_arConfig(node->oid_component, "min_speed", data);

    if (strlen(data) == 0)
    {
        strcpy(data, "0");
    }

    #if (LOG_LEVEL <= LOG_LEVEL_INFO)
        LOG_FATAL("get_autoRstSpeedDown Port = %d", node->oid_component);
        LOG_FATAL("DATA\n --  strlen = %lu\n", strlen(data));
        LOG_FATAL("%s", data);
    #else
        printf("%s\n", data);
    #endif
}

void get_autoRstSpeedUp(StaticTreeNode *node)
{
    char data[256];
    get_arConfig(node->oid_component, "max_speed", data);

    if (strlen(data) == 0)
    {
        strcpy(data, "0");
    }

    #if (LOG_LEVEL <= LOG_LEVEL_INFO)
        LOG_FATAL("get_autoRstSpeedUp Port = %d", node->oid_component);
        LOG_FATAL("DATA\n --  strlen = %lu\n", strlen(data));
        LOG_FATAL("%s", data);
    #else
        printf("%s\n", data);
    #endif
}

void get_autoReStartTimeOnHour(StaticTreeNode *node)
{
    char data[256];
    get_arConfig(node->oid_component, "timeUp", data);

    if (strlen(data) == 0)
    {
        strcpy(data, "0");
    }

#if (LOG_LEVEL <= LOG_LEVEL_INFO)
    LOG_FATAL("get_autoReStartTimeOnHour Port = %d", node->oid_component);
    LOG_FATAL("DATA\n --  strlen = %lu\n", strlen(data));
    LOG_FATAL("%s", data);
#else
    printf("%s\n", data);
#endif
}

void get_autoReStartTimeOnMin(StaticTreeNode *node)
{
    char data[256];
    get_arConfig(node->oid_component, "timeUp", data);

    if (strlen(data) == 0)
    {
        strcpy(data, "0");
    }

    #if (LOG_LEVEL <= LOG_LEVEL_INFO)
        LOG_FATAL("get_autoReStartTimeOnMin Port = %d", node->oid_component);
        LOG_FATAL("DATA\n --  strlen = %lu\n", strlen(data));
        LOG_FATAL("%s", data);
    #else
        printf("%s\n", data);
    #endif

}

void get_autoReStartTimeOffHour(StaticTreeNode *node)
{
    char data[256];
    get_arConfig(node->oid_component, "timeDown", data);

    if (strlen(data) == 0)
    {
        strcpy(data, "0");
    }

    #if (LOG_LEVEL <= LOG_LEVEL_INFO)
        LOG_FATAL("get_autoReStartTimeOffHour Port = %d", node->oid_component);
        LOG_FATAL("DATA\n --  strlen = %lu\n", strlen(data));
        LOG_FATAL("%s", data);
    #else
        printf("%s\n", data);
    #endif
}

void get_autoReStartTimeOffMin(StaticTreeNode *node)
{
    char data[256];
    get_arConfig(node->oid_component, "timeDown", data);

    if (strlen(data) == 0)
    {
        strcpy(data, "0");
    }

    #if (LOG_LEVEL <= LOG_LEVEL_INFO)
        LOG_FATAL("get_autoReStartTimeOffMin Port = %d", node->oid_component);
        LOG_FATAL("DATA\n --  strlen = %lu\n", strlen(data));
        LOG_FATAL("%s", data);
    #else
        printf("%s\n", data);
    #endif
}

uint16_t init_mib_autoRestart(uint16_t parent_index)
{
    uint16_t autoRestart = add_node(2, "autoRestart", NODE_INTERNAL, parent_index, NULL, NULL);

    uint16_t ar_table = add_node(1, "autoRestartTable", NODE_INTERNAL, autoRestart, NULL, NULL);
    uint16_t ar_entry = add_node(1, "autoRestartEntry", NODE_INTERNAL, ar_table, NULL, NULL);

    HandlerFunc arHandlers[] = {
        get_autoRstIndex,
        get_autoRstMode,
        get_autoRstDstIP,
        get_autoRstSpeedDown,
        get_autoRstSpeedUp,
        get_autoReStartTimeOnHour,
        get_autoReStartTimeOnMin,
        get_autoReStartTimeOffHour,
        get_autoReStartTimeOffMin
    };

    for(int col = 0; col < AUTO_RESTART_COL; col++)
    {
        uint16_t col_node = add_node(col+1, ar_columns[col], NODE_INTERNAL, ar_entry, NULL, NULL);

        // Добавляем порты
        for(int port = 1; port <= POE_PORTS; port++)
        {
            void *data = NULL;
            NodeType nodeType = col == 2 ? NODE_LEAF_IP : NODE_LEAF_INT;
            add_node(port, "arConfig_portEntry", nodeType, col_node, data, arHandlers[col]);
        }
    }
    return ar_entry;
}

static int get_arConfig(uint16_t port, char* option, char *res)
{
    if (port < 1 || port > 8)
        return 1;

    char command[128];
    snprintf(command, sizeof(command), "ubus call uci get '{\"config\":\"tf_autorestart\", \"section\":\"lan%d\", \"option\":\"%s\"}'",port, option);
    LOG_DEBUG("command = %s", command);
    FILE* fp = popen(command, "r");
    if (!fp)
        return 2;

    size_t len = fread(res, 1, 256, fp);
    res[len] = '\0';
    pclose(fp);
    return 0;
}


