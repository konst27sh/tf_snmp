//
// Created by sheverdin on 5/5/25.
//

#include "autoRestartErrors_maiin.h"
#include "../../utils/debug.h"

const char *arErr_columns[AUTO_RESTART_ERR_COL] =
{
    "arPortIndex",
    "arPortStatus",
};

void get_arPortIndex(StaticTreeNode *node)
{
    LOG_FATAL("get_arPortIndex");
}

void get_arPortStatus(StaticTreeNode *node)
{
    LOG_FATAL("get_arPortStatus");
}

uint16_t init_mib_autoRestartErr(uint16_t parent_index)
{
    uint16_t specialStatus = add_node(6, "specialStatus", NODE_INTERNAL, parent_index, NULL, NULL);
    uint16_t autoRestartErrors = add_node(1, "autoRestartErrors", NODE_INTERNAL, specialStatus, NULL, NULL);

    uint16_t autoRestartErrors_table = add_node(1, "autoRestartTable", NODE_INTERNAL, autoRestartErrors, NULL, NULL);
    uint16_t autoRestartErrors_entry = add_node(1, "autoRestartEntry", NODE_INTERNAL, autoRestartErrors_table, NULL, NULL);

    for(int col = 0; col < AUTO_RESTART_ERR_COL; col++)
    {
        uint16_t col_node = add_node(col+1, arErr_columns[col], NODE_INTERNAL, autoRestartErrors_entry, NULL, NULL);

        for(int port = 1; port <= POE_PORTS; port++)
        {
            void *data = NULL;
            HandlerFunc autoRestartErrorsHandlers[] = {
                get_arPortIndex,
                get_arPortStatus,
            };
            add_node(port, "arErr_portEntry", NODE_LEAF_INT, col_node, data, autoRestartErrorsHandlers[col]);
        }
    }
    return specialStatus;
}

