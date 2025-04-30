//
// Created by sheverdin on 4/30/25.
//

#include "autoRestart_main.h"
#include "../../utils/debug.h"

const char *columns[9] = {
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

uint16_t init_mib_tree_autoRestart(uint16_t parent_index)
{
    uint16_t autoRestart = add_node(2, "autoRestart", NODE_INTERNAL, parent_index, NULL, NULL);

    uint16_t table = add_node(1, "autoRestartTable", NODE_INTERNAL, autoRestart, NULL, NULL);
    uint16_t entry = add_node(1, "autoRestartEntry", NODE_INTERNAL, table, NULL, NULL);

    //for(int col = 0; col < ; col++)
    //{
    //    uint16_t col_node = add_node(col+1, columns[col], NODE_INTERNAL, poeEntry, NULL, NULL);
    //
    //    // Добавляем порты
    //    LOG_DEV("PORTS");
    //    for(int port = 1; port <= POE_PORTS; port++)
    //    {
    //        void *data = NULL;
    //        HandlerFunc poeHandlers[] = {
    //                get_poeIndex,
    //                get_poeStatus,
    //                get_poePower
    //        };
    //        add_node(port, "portEntry", NODE_LEAF_INT, col_node, data, poeHandlers[col]);
    //    }
    //}
    return 0;
}

