//
// Created by sheverdin on 4/18/25.
//

#ifndef TF_SNMP_UTILS_H
#define TF_SNMP_UTILS_H

#include <jansson.h>
#include <stdint.h>

json_t* getData_formJson(char* json_data);
uint16_t int_to_string(int num, char *str);


#endif //TF_SNMP_UTILS_H
