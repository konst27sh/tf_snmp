//
// Created by sheverdin on 5/5/25.
//

#include "fwStatus_main.h"
#include "../../utils/debug.h"
#include "../../utils/utils.h"

typedef enum {
    fwVersion = 1,
}SENSOR_ENTRY_e;

void get_fwStatus(StaticTreeNode *node)
{
    LOG_DEBUG("get_fwStatus");
    char result[MAX_BUFFER_SIZE] = {0};
    char data[384] = {0};
    char command[128];
    snprintf(command, sizeof(command), "ubus call system board");
    get_ubus_Data(command, data, 512);
    get_json_data(data, "release", "version", result);
    printf("%s\n", result);
}

uint16_t init_mib_fwStatus(uint16_t parent_index)
{
    HandlerFunc fwStatusHandlers[] =
    {
            NULL,
            get_fwStatus,
    };

    uint16_t fwStatus = add_node(3, "fwStatus", NODE_INTERNAL, parent_index, NULL, NULL);
    uint16_t fwVersion_index = add_node(fwVersion, "fwVersion", NODE_LEAF_INT, fwStatus, NULL, NULL);
    add_node(0, "fwVersion_scalar", NODE_LEAF_STRING, fwVersion_index, NULL, fwStatusHandlers[fwVersion]);

    return fwStatus;
}
