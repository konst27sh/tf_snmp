//
// Created by sheverdin on 4/30/25.
//

#include "string.h"
#include "autoRestart_main.h"
#include "../../utils/debug.h"
#include "../../utils/utils.h"


#define AR_MODE (4)
#define TIME_STR_LEN (6)

static int get_arConfig(uint16_t port, const char* option, char *res);
static void parse_time(const char *time_str, char *hour, char *minute);
static void handle_generic_get(StaticTreeNode *node, const char *option,
                               NodeType nodeType, const char *default_val);

typedef struct
{
    const char *option;
    NodeType nodeType;
    const char *defaultValue;
} ColumnConfig;

static const ColumnConfig ar_columns_config[AUTO_RESTART_COL] =
{
    {"autoRstIndex", NODE_LEAF_INT,  NULL},
    {"mode",         NODE_LEAF_INT,  "disabled"},
    {"host",         NODE_LEAF_IP,   "0.0.0.0"},
    {"min_speed",    NODE_LEAF_INT,  "0"},
    {"max_speed",    NODE_LEAF_INT,  "0"},
    {"timeUp",       NODE_LEAF_INT, "00"},
    {"timeUp",       NODE_LEAF_INT, "00"},
    {"timeDown",     NODE_LEAF_INT, "00"},
    {"timeDown",     NODE_LEAF_INT, "00"}
};

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

void get_autoRstMode(StaticTreeNode *node)
{
    handle_generic_get(node, "mode",      NODE_LEAF_INT,  "disabled");
}

void get_autoRstDstIP(StaticTreeNode *node)
{
    handle_generic_get(node, "host",      NODE_LEAF_IP,   "255.255.255.255");
}
void get_autoRstSpeedDown(StaticTreeNode *node)
{
    handle_generic_get(node, "min_speed", NODE_LEAF_INT,  "0");
}

void get_autoRstSpeedUp(StaticTreeNode *node)
{
    handle_generic_get(node, "max_speed", NODE_LEAF_INT,  "0");
}

void get_autoReStartTimeOnHour(StaticTreeNode *node)
{
    handle_generic_get(node, "timeUp", NODE_LEAF_INT, "00");
}
void get_autoReStartTimeOnMin(StaticTreeNode *node)
{
    handle_generic_get(node,    "timeUp",    NODE_LEAF_INT, "00");
}
void get_autoReStartTimeOffHour(StaticTreeNode *node)
{
    handle_generic_get(node,    "timeDown",  NODE_LEAF_INT, "00");
}
void get_autoReStartTimeOffMin(StaticTreeNode *node)
{
    handle_generic_get(node,    "timeDown",  NODE_LEAF_INT, "00");
}

void get_autoRstIndex(StaticTreeNode *node)
{
    printf("%d\n", node->oid_component);
}

#if 0
void get_autoRstMode(StaticTreeNode *node)
{
    char data[256];
    get_arConfig(node->oid_component, "mode", data);
    AR_MODE_e arMode = disabled;
    char status_v[] = "";

    get_string_data(data, status_v);

    if (status_v != NULL)
    {
        for (AR_MODE_e i = 0; i < AR_MODE; i++) {
            if (strcmp(status_v, ar_mode[i]) == 0) {
                arMode = i;
                break;
            }
        }
    }

    #if (LOG_LEVEL <= LOG_LEVEL_INFO)
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
    char status_v[] = "0.0.0.0";

    get_string_data(data, status_v);

    #if (LOG_LEVEL <= LOG_LEVEL_INFO)
        LOG_FATAL("get_autoRstDstIP Port = %d", node->oid_component);
        LOG_FATAL("host: %s", status_v);
        LOG_FATAL("DATA\n --  strlen = %lu\n", strlen(status_v));
    #else
        printf("%s\n", status_v);
    #endif
}

void get_autoRstSpeedDown(StaticTreeNode *node)
{
    char data[256];
    get_arConfig(node->oid_component, "min_speed", data);
    char status_v[] = "0";
    get_string_data(data, status_v);

    #if (LOG_LEVEL <= LOG_LEVEL_INFO)
        LOG_FATAL("get_autoRstSpeedDown Port = %d", node->oid_component);
        LOG_FATAL("speed: %s", status_v);
        LOG_FATAL("data: %s", data);

    #else
        printf("%s\n", status_v);
    #endif
}

void get_autoRstSpeedUp(StaticTreeNode *node)
{
    char data[256];
    get_arConfig(node->oid_component, "max_speed", data);

    char status_v[] = "0";
    get_string_data(data, status_v);

    #if (LOG_LEVEL <= LOG_LEVEL_INFO)
        LOG_FATAL("get_autoRstSpeedUp Port = %d", node->oid_component);
        LOG_FATAL("speed max: %s", status_v);
    #else
        printf("%s\n", status_v);
    #endif
}

void get_autoReStartTimeOnHour(StaticTreeNode *node)
{
    char data[256];
    get_arConfig(node->oid_component, "timeUp", data);
    char status_v[6] = "00:00\n";
    get_string_data(data, status_v);

    char hour[3] = "00\n";
    char min[3] = "00\n";
    LOG_FATAL("Time Off status_v: %s", status_v);
    parse_time(status_v, hour, min);

    #if (LOG_LEVEL <= LOG_LEVEL_INFO)
        LOG_FATAL("get_autoReStartTimeOnHour Port = %d", node->oid_component);
        LOG_FATAL("DATA\n --  strlen = %lu\n", strlen(data));
        LOG_FATAL("Time On  : %s", status_v);
        LOG_FATAL("Time On min : %s -- hour = %s", min, hour);
    #else
        printf("%s\n", hour);
    #endif
}

void get_autoReStartTimeOnMin(StaticTreeNode *node)
{
    char data[256];
    get_arConfig(node->oid_component, "timeUp", data);
    char status_v[6] = "00:00\n";
    get_string_data(data, status_v);

    char hour[3] = "00\n";
    char min[3] = "00\n";
    parse_time(status_v, hour, min);

    #if (LOG_LEVEL <= LOG_LEVEL_INFO)
        LOG_FATAL("get_autoReStartTimeOnMin Port = %d", node->oid_component);
        LOG_FATAL("Time On MIN : %s -- min = %s", status_v, min);
    #else
        printf("%s\n", min);
    #endif
}

void get_autoReStartTimeOffHour(StaticTreeNode *node)
{
    char data[256];
    get_arConfig(node->oid_component, "timeDown", data);

    char status_v[6] = "00:00\n";
    get_string_data(data, status_v);

    char hour[3] = "00\n";
    char min[3] = "00\n";
    parse_time(status_v, hour, min);
    #if (LOG_LEVEL <= LOG_LEVEL_INFO)
        LOG_FATAL("get_autoReStartTimeOffHour Port = %d", node->oid_component);
        LOG_FATAL("DATA\n --  strlen = %lu\n", strlen(data));
        LOG_FATAL("Time Off Hour %s -- hour = %s", status_v, hour);
    #else
        printf("%s\n", hour);
    #endif
}

void get_autoReStartTimeOffMin(StaticTreeNode *node)
{
    char data[256];
    get_arConfig(node->oid_component, "timeDown", data);

    char status_v[6] = "00:00\n";
    get_string_data(data, status_v);
    char hour[3] = "00\n";
    char min[3] = "00\n";
    parse_time(status_v, hour, min);

    #if (LOG_LEVEL <= LOG_LEVEL_INFO)
        LOG_FATAL("get_autoReStartTimeOffMin Port = %d", node->oid_component);
        LOG_FATAL("DATA\n --  strlen = %lu\n", strlen(data));
        LOG_FATAL("Time Off Min: %s -- min = %s", status_v, min);
    #else
        printf("%s\n", min);
    #endif
}
#endif

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

        for(int port = 1; port <= POE_PORTS; port++)
        {
            void *data = NULL;
            NodeType nodeType = col == 2 ? NODE_LEAF_IP : NODE_LEAF_INT;
            add_node(port, ar_columns[col],  ar_columns_config[col].nodeType, col_node, data, arHandlers[col]);
        }
    }
    return ar_entry;
}

static void handle_generic_get(StaticTreeNode *node, const char *option,
                               NodeType nodeType, const char *default_val)
{
    char data[MAX_BUFFER_SIZE] = {0};
    char result[MAX_BUFFER_SIZE] = {0};
    strncpy(result, default_val, MAX_BUFFER_SIZE-1);
    AR_MODE_e  arMode = disabled;
    get_arConfig(node->oid_component, option, data);
    get_string_data(data, result, "value");

    if (strcmp(node->name, "autoRstMode") == 0)
    {
        for (AR_MODE_e i = 0; i < AR_MODE; i++) {
            if (strcmp(result, ar_mode[i]) == 0) {
                arMode = i;
                break;
            }
        }
        printf("%d\n", arMode);
    }
    else if (strstr(node->name, "autoReStartTime"))
    {
        char hour[3], min[3];
        strcpy(hour, default_val);
        strcpy(min, default_val);
        hour[2] = '\0';
        min[2]  = '\0';
        parse_time(result, hour, min);
        printf("%s\n", (strstr(node->name, "Hour")) ? hour : min);
    }
    else
    {
        printf("%s\n", result);
    }
}

static int get_arConfig(uint16_t port, const char* option, char *res)
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

static void parse_time(const char *time_str, char *hour, char *minute)
{
    char copy[TIME_STR_LEN];
    strncpy(copy, time_str, TIME_STR_LEN-1);
    copy[TIME_STR_LEN-1] = '\0';

    char *ptr = strtok(copy, ":");
    if (ptr != NULL)
        strncpy(hour, ptr, 2);
    ptr = strtok(NULL, ":");
    if (ptr != NULL )
        strncpy(minute, ptr, 2);
    hour[2] = minute[2] = '\0';
}

//static void get_string_data(char *data, char *res)
//{
//    json_t *root = NULL;
//    json_t *value = NULL;
//    char *res_temp = NULL;
//
//    if (strlen(data) != 0)
//    {
//        root = getData_formJson(data);
//    }
//    if (root != NULL)
//    {
//        value = json_object_get(root, "value");
//        if (value != NULL)
//        {
//            if json_is_string(value)
//            {
//                res_temp = (char *)json_string_value(value);
//            }
//        }
//    }
//
//    if (res_temp != NULL)
//    {
//        strncpy(res, res_temp, MAX_BUFFER_SIZE-1);
//    }
//}

