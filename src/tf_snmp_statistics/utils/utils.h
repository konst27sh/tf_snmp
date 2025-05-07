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
int get_i2c_Data(const char* i2c_name, char *res);
void get_string_data(char *data, char *res, const char *option);
int get_ubus_Data(const char* command, char *res, uint16_t ubusSize);
void get_json_data(char *data, const char *option,  const char *section, char *res);

uint16_t int_to_string(int num, char *str);


#endif //TF_SNMP_UTILS_H
