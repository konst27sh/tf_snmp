//
// Created by sheverdin on 5/5/25.
//

#ifndef TF_SNMP_INPUTSTATUS_MAIN_H
#define TF_SNMP_INPUTSTATUS_MAIN_H

#include "../../tf_snmp_module.h"

#define INPUT_STATUS_COL (4)
#define INPUT_STATUS_PORTS (3)

uint16_t init_mib_inputStatus(uint16_t parent_index);

#endif //TF_SNMP_INPUTSTATUS_MAIN_H
