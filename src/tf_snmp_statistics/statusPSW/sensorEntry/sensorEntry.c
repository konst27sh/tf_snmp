//
// Created by sheverdin on 5/5/25.
//

#include "sensorEntry.h"
#include "../../utils/debug.h"

typedef enum {
    sensorConnected = 1,
    sensorTemperature,
    sensorHumidity
}SENSOR_ENTRY_e;

void get_sensorConnected(StaticTreeNode *node)
{
    LOG_FATAL("get_sensorConnected");
}

void get_sensorTemperature(StaticTreeNode *node)
{
    LOG_FATAL("get_sensorTemperature");
}

void get_sensorHumidity(StaticTreeNode *node)
{
    LOG_FATAL("get_sensorHumidity");
}

uint16_t init_mib_sensorEntry(uint16_t parent_index)
{
    HandlerFunc sensorEntryHandlers[] = {
        NULL,
        get_sensorConnected,
        get_sensorTemperature,
        get_sensorHumidity,
    };

    uint16_t sensorEntry = add_node(8, "sensorEntry", NODE_INTERNAL, parent_index, NULL, NULL);

    uint16_t sensorConnected_index = add_node(sensorConnected, "sensorConnected", NODE_INTERNAL, sensorEntry, NULL, NULL);
    add_node(0, "sensorConnected_scalar", NODE_LEAF_INT, sensorConnected_index, NULL, sensorEntryHandlers[sensorConnected]);

    uint16_t sensorTemperature_index = add_node(sensorTemperature, "sensorTemperature", NODE_INTERNAL, sensorEntry, NULL, NULL);
    add_node(0, "sensorTemperature_scalar", NODE_LEAF_INT, sensorTemperature_index, NULL, sensorEntryHandlers[sensorTemperature]);

    uint16_t sensorHumidity_index = add_node(sensorHumidity, "sensorHumidity", NODE_INTERNAL, sensorEntry, NULL, NULL);
    add_node(0, "sensorHumiditye_scalar", NODE_LEAF_INT, sensorHumidity_index, NULL, sensorEntryHandlers[sensorHumidity]);

    return sensorEntry;
}
