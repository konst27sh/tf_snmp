//
// Created by sheverdin on 4/22/25.
//

#ifndef TF_SNMP_POE_HANDLERS_H
#define TF_SNMP_POE_HANDLERS_H

#include "../../tf_snmp_module.h"

void get_poeIndex(StaticTreeNode *node);
void get_poeStatus(StaticTreeNode *node);
void get_poePower(StaticTreeNode *node);

#endif //TF_SNMP_POE_HANDLERS_H
