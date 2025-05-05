//
// Created by sheverdin on 5/5/25.
//

#include "outStatePSW_main.h"
#include "../../utils/debug.h"

typedef enum {
    state = 1,
}outStatePSW_ENTRY_e;

void get_outStatePSW(StaticTreeNode *node)
{
    LOG_FATAL("get_autoRstIndex");
}

uint16_t init_mib_outStatePSW(uint16_t parent_index)
{
    HandlerFunc outStatePSW_stHandlers[] = {
        NULL,
        get_outStatePSW,
    };

    uint16_t outState = add_node(4, "outStatePSW", NODE_INTERNAL, parent_index, NULL, NULL);
    uint16_t outState_node = add_node(state, "state", NODE_LEAF_INT, outState, NULL, NULL);
    add_node(0, "stateScalar", NODE_LEAF_INT, outState_node, NULL, outStatePSW_stHandlers[state]); // .0
    return outState;
}

