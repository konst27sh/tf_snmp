//
// Created by sheverdin on 4/30/25.
//

#include "statusPSW_main.h"

uint16_t init_mib_statusPSW(uint16_t parent)
{
    uint16_t statusPSW = add_node(1, "statusPSW", NODE_INTERNAL, parent, NULL, NULL);
    return statusPSW;
}