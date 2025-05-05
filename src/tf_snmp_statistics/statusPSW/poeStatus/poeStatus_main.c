//
// Created by sheverdin on 4/21/25.
//

#include "poeStatus_main.h"
#include "../../utils/debug.h"

#define POE_STATUS_COL (3)

const char *poe_columns[POE_STATUS_COL] =
{
    "portPoeStatusIndex",
    "portPoeStatusState",
    "portPoeStatusPower"
};

// Инициализация дерева согласно MIB
uint16_t init_mib_tree_poeStatus(uint16_t parent_index)
{
    int statusPSW = parent_index;
    uint16_t poeStatus = add_node(5, "poeStatus", NODE_INTERNAL, statusPSW, NULL, NULL);

    // Таблица poeStatus
    uint16_t poeTable = add_node(1, "poeStatusTable", NODE_INTERNAL, poeStatus, NULL, NULL);
    uint16_t poeEntry = add_node(1, "poeStatusEntry", NODE_INTERNAL, poeTable, NULL,NULL);

    // Колонки таблицы
    for(int col = 0; col < POE_STATUS_COL; col++)
    {
        uint16_t col_node = add_node(col+1, poe_columns[col], NODE_INTERNAL, poeEntry, NULL, NULL);

        for(int port = 1; port <= POE_PORTS; port++)
        {
            void *data = NULL;
            HandlerFunc poeStatusHandlers[] = {
                get_poeIndex,
                get_poeStatus,
                get_poePower
            };
            add_node(port, "PoEStatus_portEntry", NODE_LEAF_INT, col_node, data, poeStatusHandlers[col]);
        }
    }
    return poeEntry;
}




