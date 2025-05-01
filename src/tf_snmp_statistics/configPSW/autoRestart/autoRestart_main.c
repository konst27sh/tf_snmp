//
// Created by sheverdin on 4/30/25.
//

#include "autoRestart_main.h"
#include "../../utils/debug.h"

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

void get_autoRstIndex(StaticTreeNode *node)
{
    LOG_FATAL("get_autoRstIndex");
}

void get_autoRstMode(StaticTreeNode *node)
{
    LOG_FATAL("get_autoRstMode");
}

void get_autoRstDstIP(StaticTreeNode *node)
{
    LOG_FATAL("get_autoRstDstIP");
}
void get_autoRstSpeedDown(StaticTreeNode *node)
{
    LOG_FATAL("get_autoRstSpeedDown");
}

void get_autoRstSpeedUp(StaticTreeNode *node)
{
    LOG_FATAL("get_autoRstSpeedUp");
}

void get_autoReStartTimeOnHour(StaticTreeNode *node)
{
    LOG_FATAL("get_autoReStartTimeOnHour");
}

void get_autoReStartTimeOnMin(StaticTreeNode *node)
{
    LOG_FATAL("get_autoReStartTimeOnMin");
}

void get_autoReStartTimeOffHour(StaticTreeNode *node)
{
    LOG_FATAL("get_autoReStartTimeOffHour");
}

void get_autoReStartTimeOffMin(StaticTreeNode *node)
{
    LOG_FATAL("get_autoReStartTimeOffMin");
}

uint16_t init_mib_tree_autoRestart(uint16_t parent_index)
{
    uint16_t autoRestart = add_node(2, "autoRestart", NODE_INTERNAL, parent_index, NULL, NULL);

    uint16_t ar_table = add_node(1, "autoRestartTable", NODE_INTERNAL, autoRestart, NULL, NULL);
    uint16_t ar_entry = add_node(1, "autoRestartEntry", NODE_INTERNAL, ar_table, NULL, NULL);

    for(int col = 0; col < AUTO_RESTART_COL; col++)
    {
        uint16_t col_node = add_node(col+1, ar_columns[col], NODE_INTERNAL, ar_entry, NULL, NULL);

        // Добавляем порты
        LOG_DEV("PORTS");
        for(int port = 1; port <= POE_PORTS; port++)
        {
            void *data = NULL;
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
            add_node(port, "portEntry", NODE_LEAF_INT, col_node, data, arHandlers[col]);
        }
    }
    return 0;
}

