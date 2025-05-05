//
// Created by sheverdin on 5/5/25.
//

#include "ups_status_main.h"
#include "../../utils/debug.h"

typedef enum {
    upsModeAvalible = 1,
    upsPwrSource,
    upsBatteryVoltage,
    upsBatteryTime
}SENSOR_ENTRY_e;


void get_upsModeAvalible(StaticTreeNode *node)
{
    LOG_FATAL("get_upsModeAvalible");
}

void get_upsPwrSource(StaticTreeNode *node)
{
    LOG_FATAL("get_upsPwrSource");
}

void get_upsBatteryVoltage(StaticTreeNode *node)
{
    LOG_FATAL("get_upsBatteryVoltage");
}

void get_upsBatteryTime(StaticTreeNode *node)
{
    LOG_FATAL("get_upsBatteryTime");
}

uint16_t init_mib_upsStatus(uint16_t parent_index)
{
    HandlerFunc upsHandlers[] = {
            NULL,
            get_upsModeAvalible,
            get_upsPwrSource,
            get_upsBatteryVoltage,
            get_upsBatteryTime,
    };

    uint16_t upsStatus = add_node(1, "upsStatus", NODE_INTERNAL, parent_index, NULL, NULL);

    uint16_t upsModeAvalible_index = add_node(upsModeAvalible, "upsModeAvalible", NODE_LEAF_INT, upsStatus, NULL, NULL);
    add_node(0, "upsModeAvalible_scalar", NODE_LEAF_INT, upsModeAvalible_index, NULL, upsHandlers[upsModeAvalible]);

    uint16_t upsPwrSource_index = add_node(upsPwrSource, "upsPwrSource", NODE_LEAF_INT, upsStatus, NULL, NULL);
    add_node(0, "upsPwrSource_scalar", NODE_LEAF_INT, upsPwrSource_index, NULL, upsHandlers[upsPwrSource]);

    uint16_t upsBatteryVoltage_index = add_node(upsBatteryVoltage, "upsBatteryVoltage", NODE_LEAF_INT, upsStatus, NULL, NULL);
    add_node(0, "upsBatteryVoltage_scalar", NODE_LEAF_INT, upsBatteryVoltage_index, NULL, upsHandlers[upsBatteryVoltage]);

    uint16_t upsBatteryTime_index  = add_node(upsBatteryTime, "upsBatteryTime", NODE_LEAF_INT, upsStatus, NULL, NULL);
    add_node(0, "upsBatteryTime_scalar", NODE_LEAF_INT, upsBatteryTime_index, NULL, upsHandlers[upsBatteryTime]);

    return upsStatus;
}