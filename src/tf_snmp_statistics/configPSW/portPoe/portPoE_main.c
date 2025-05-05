//
// Created by sheverdin on 5/5/25.
//

#include "portPoE_main.h"
#include "../../utils/debug.h"

const char *portPoE_columns[PORT_POE_COL] = {
    "portPoeIndex",
    "portPoeState",
};

void get_portPoeIndex(StaticTreeNode *node)
{
    LOG_FATAL("get_autoRstIndex");
}

void get_portPoeState(StaticTreeNode *node)
{
    LOG_FATAL("get_autoRstMode");
}


uint16_t init_mib_portPoE(uint16_t parent_index)
{
    uint16_t autoRestart = add_node(3, "portPoe", NODE_INTERNAL, parent_index, NULL, NULL);

    uint16_t portPoE_table = add_node(1, "portPoeTable", NODE_INTERNAL, autoRestart, NULL, NULL);
    uint16_t portPoE_entry = add_node(1, "portPoeEntry", NODE_INTERNAL, portPoE_table, NULL, NULL);

    for(int col = 0; col < PORT_POE_COL; col++)
    {
        uint16_t col_node = add_node(col+1, portPoE_columns[col], NODE_INTERNAL, portPoE_entry, NULL, NULL);

        // Добавляем порты
        for(int port = 1; port <= POE_PORTS; port++)
        {
            void *data = NULL;
            HandlerFunc portPoEHandlers[] = {
                get_portPoeIndex,
                get_portPoeState,
            };
            add_node(port, "portPoE_portEntry", NODE_LEAF_INT, col_node, data, portPoEHandlers[col]);
        }
    }
    return portPoE_entry;
}

