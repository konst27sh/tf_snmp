//
// Created by sheverdin on 4/30/25.
//

#include "configPSW_main.h"

uint16_t init_mib_configPSW(uint16_t parent)
{
    uint16_t configPSW = add_node(1, "configPSW", NODE_INTERNAL, parent, NULL, NULL);
    return configPSW;
}