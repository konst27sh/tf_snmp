//
// Created by sheverdin on 4/18/25.
//

#ifndef TF_SNMP_UTILS_H
#define TF_SNMP_UTILS_H

#include <jansson.h>
#include <stdint.h>
#include "string.h"

#define MAX_BUFFER_SIZE 256

json_t* getData_formJson(char* json_data);
void get_string_data(char *data, char *res, const char *option);
uint16_t int_to_string(int num, char *str);


#endif //TF_SNMP_UTILS_H
