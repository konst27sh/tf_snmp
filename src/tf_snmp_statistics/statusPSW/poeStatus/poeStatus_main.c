//
// Created by sheverdin on 4/21/25.
//


#include "../../utils/debug.h"
#include "poeStatus_main.h"


// Инициализация дерева согласно MIB
uint16_t init_mib_tree_poeStatus(uint16_t parent_index)
{
    int statusPSW = parent_index;
    //uint16_t statusPSW = add_node(2, "statusPSW", NODE_INTERNAL, psw, NULL, NULL);
    uint16_t poeStatus = add_node(5, "poeStatus", NODE_INTERNAL, statusPSW, NULL, NULL);

    // Таблица poeStatus
    LOG_DEV("Status");
    uint16_t poeTable = add_node(1, "poeStatusTable", NODE_INTERNAL, poeStatus, NULL, NULL);
    uint16_t poeEntry = add_node(1, "poeStatusEntry", NODE_INTERNAL, poeTable, NULL,NULL);

    // Колонки таблицы
    LOG_DEV("table");
    const char *poe_columns[] = {"portPoeStatusIndex", "portPoeStatusState", "portPoeStatusPower"};
    for(int col = 0; col < 3; col++)
    {
        uint16_t col_node = add_node(col+1, poe_columns[col], NODE_INTERNAL, poeEntry, NULL, NULL);

        // Добавляем порты
        LOG_DEV("PORTS");
        for(int port = 1; port <= POE_PORTS; port++)
        {
            void *data = NULL;
            HandlerFunc poeHandlers[] = {
                get_poeIndex,
                get_poeStatus,
                get_poePower
            };
            add_node(port, "portEntry", NODE_LEAF_INT, col_node, data, poeHandlers[col]);
        }
    }
    return poeEntry;
}




