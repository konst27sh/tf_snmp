//
// Created by sheverdin on 5/5/25.
//

#include "inputStatus_main.h"
#include "../../utils/debug.h"

const char *inputState_columns[INPUT_STATUS_COL] = {
    "inputIndex",
    "inputType",
    "autoRstDstIP",
    "inputState",
};

#define INPUT_STATUS_PORTS (4)


void get_inputIndex(StaticTreeNode *node)
{
    LOG_FATAL("get_inputIndex");
}

void get_inputType(StaticTreeNode *node)
{
    LOG_FATAL("get_inputType");
}

void get_inputState(StaticTreeNode *node)
{
    LOG_FATAL("get_inputState");
}
void get_inputAlarm(StaticTreeNode *node)
{
    LOG_FATAL("get_inputAlarm");
}

uint16_t init_mib_inputStatus(uint16_t parent_index)
{
    uint16_t inputStatus = add_node(2, "inputStatus", NODE_INTERNAL, parent_index, NULL, NULL);

    uint16_t inputStatus_table = add_node(1, "inputStatusTable", NODE_INTERNAL, inputStatus, NULL, NULL);
    uint16_t inputStatus_entry = add_node(1, "inputStatusEntry", NODE_INTERNAL, inputStatus_table, NULL, NULL);

    for(int col = 0; col < INPUT_STATUS_COL; col++)
    {
        uint16_t col_node = add_node(col+1, inputState_columns[col], NODE_INTERNAL, inputStatus_entry, NULL, NULL);

        for(int port = 1; port <= INPUT_STATUS_PORTS; port++)
        {
            void *data = NULL;
            HandlerFunc inputStatusHandlers[] = {
                get_inputIndex,
                get_inputType,
                get_inputState,
                get_inputAlarm,
            };
            add_node(port, "portEntry", NODE_LEAF_INT, col_node, data, inputStatusHandlers[col]);
        }
    }
    return inputStatus;
}

