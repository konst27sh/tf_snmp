//
// Created by sheverdin on 5/5/25.
//

#include "ups_status_main.h"
#include "../../utils/debug.h"
#include "../../utils/utils.h"

typedef enum {
    upsModeAvalible = 1,
    upsPwrSource,
    upsBatteryVoltage,
    upsBatteryTime
}SENSOR_ENTRY_e;

static void get_upsStatus(StaticTreeNode *node);

uint16_t init_mib_upsStatus(uint16_t parent_index)
{

    uint16_t upsStatus = add_node(1, "upsStatus", NODE_INTERNAL, parent_index, NULL, NULL);

    uint16_t upsModeAvalible_index = add_node(upsModeAvalible, "upsModeAvalible", NODE_LEAF_INT, upsStatus, NULL, NULL);
    add_node(0, "upsModeAvalible_scalar", NODE_LEAF_INT, upsModeAvalible_index, NULL, get_upsStatus);

    uint16_t upsPwrSource_index = add_node(upsPwrSource, "upsPwrSource", NODE_LEAF_INT, upsStatus, NULL, NULL);
    add_node(0, "upsPwrSource_scalar", NODE_LEAF_INT, upsPwrSource_index, NULL, get_upsStatus);

    uint16_t upsBatteryVoltage_index = add_node(upsBatteryVoltage, "upsBatteryVoltage", NODE_LEAF_INT, upsStatus, NULL, NULL);
    add_node(0, "upsBatteryVoltage_scalar", NODE_LEAF_INT, upsBatteryVoltage_index, NULL, get_upsStatus);

    uint16_t upsBatteryTime_index  = add_node(upsBatteryTime, "upsBatteryTime", NODE_LEAF_INT, upsStatus, NULL, NULL);
    add_node(0, "upsBatteryTime_scalar", NODE_LEAF_INT, upsBatteryTime_index, NULL, get_upsStatus);

    return upsStatus;
}

static void get_upsStatus(StaticTreeNode *node)
{
    LOG_DEBUG("get_upsModeAvalible");
    LOG_DEBUG("node->name %s", node->name);
    char data[MAX_BUFFER_SIZE] = {0};
    char result[MAX_BUFFER_SIZE] = {0};
    char copy[24];
    strncpy(copy, node->name, 23);
    copy[23] = '\0';
    char *upsName = strtok(copy, "_");
    LOG_DEBUG("upsName %s", upsName);
    get_i2c_Data(upsName, data);
    get_string_data(data, result, upsName);
    LOG_DEBUG("data :");
    LOG_DEBUG("%s", data);
    LOG_DEBUG("result = %s", result);
    printf("%s\n", result);
}

