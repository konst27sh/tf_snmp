//
// Created by sheverdin on 4/16/25.
//

#include "poe_module.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jansson.h"
#include "snmp_utils.h"

#define MAX_BUFFER_SIZE 4096

const char poeStatus_table[3][32] =
{
    {""},                   //  0
    {"Delivering power"},   //  1
    {"Searching"},          //  2
};

const char poeStatus_value[3][2] =
{
    {""},                   //  0
    {"1"},   //  1
    {"2"},          //  2
};

static int convert_poeStatus(const char *str)
{
    int res = 0;
    for (int i = 0; i < 3; i++)
    {
        if (strcmp(str, poeStatus_table[i]) == 0)
        {
            res = i;
            break;
        }
    }
    return res;
}

int getData_formJson(char* json_data,  char *section, oid last_index, char *data)
{
    json_t *root;
    json_error_t error;
    int error_code = 0;
    char *value_str = NULL;
    root = json_loads(json_data, 0, &error);
    if (!root) {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        return 1;
    }
    json_t *ports = json_object_get(root, "ports");
    if (ports == NULL)
    {
        return 2;
    }
    json_t *section_data = json_object_get(ports, section);
    if (section_data == NULL)
    {
        return 4;
    }

    json_t *value = json_object_get(section_data, "status");
    if (value == NULL)
    {
        value_str = "";
        error_code = 5;
    }
    value_str = (char *)json_string_value(value);
    //printf("status = %s\n", value_str);
    if (last_index == 2)
    {
        strcpy(data, value_str);
    }
    else if (last_index == 3)
    {
        //printf("get power\n");
        value = json_object_get(section_data, "power");
        if (value == NULL) {
            printf("value == NULL\n");
            error_code = 6;
            value_str = "0";
        }
        else
        {
            //printf("value type = %d\n", value->type);
            if (value->type == JSON_INTEGER || value->type == JSON_REAL)
            {
                int res_power = (int)json_number_value(value);
                //printf("res_power = %d\n", res_power);
                int_to_str(res_power, value_str);
                strcpy(data, value_str);
            }
            //value_str = json_string_value(value);
        }
    }

    json_decref(root);
    return error_code;
}

int parse_command_output(int index, oid last_index, char *res) {
    char command[64];
    FILE *fp;
    char buffer[MAX_BUFFER_SIZE];
    char *json_str = NULL;
    size_t json_size = 0;

    // Формируем команду с индексом
    snprintf(command, sizeof(command), "ubus call poe info");

    // Выполняем команду и читаем вывод
    fp = popen(command, "r");
    if (!fp) {
        perror("Failed to run command");
        return 1;
    }

    // Читаем весь вывод команды
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        size_t len = strlen(buffer);
        char *tmp = realloc(json_str, json_size + len + 1);
        if (!tmp) {
            pclose(fp);
            free(json_str);
            printf("parse_command error 1\n");
            return 2;
        }
        json_str = tmp;
        memcpy(json_str + json_size, buffer, len);
        json_size += len;
        json_str[json_size] = '\0';
    }
    pclose(fp);

    // Формируем ключ для поиска (lanX)
    char lan_key[8];
    snprintf(lan_key, sizeof(lan_key), "lan%d", index);

    // Получаем нужный объект
    //printf("key = %s\n", lan_key);

    int res_json = getData_formJson(json_str, lan_key, last_index, res);
    if (res_json > 0)
    {
        printf("get json error = %d\n", res_json);
        return 3;
    }
    //printf("data = %s\n", res);
    return 0;
}

int snmp_getPoeStatus(TreeNode *oid_node, int port, char *poeStatus_data)
{
    char poe_buffer[32];
    oid last_index = oid_node->oid[oid_node->oid_len - 1];
    int error_code = 0;
    //printf("last_index = %lu\n", last_index);
    switch (last_index)
    {
        case 1:
            printf("%d\n", port);
            break;
        case 2:
            {
                int poe_res = parse_command_output(port, last_index, poe_buffer);
                if (poe_res == 0)
                {
                    int poe_num = convert_poeStatus(poe_buffer);
                    //printf("poe_num = %d\n", poe_num);
                    strcpy(poeStatus_data, poeStatus_value[poe_num]);
                }
                else
                {
                    error_code = 1;
                }
            }
            break;
        case 3:
            //printf("POWER\n");
            {
                int poe_res = parse_command_output(port, last_index, poeStatus_data);
                if (poe_res > 0)
                {
                    error_code = 1;
                }
            }
            break;
        default:
            error_code = 1;
            printf("POE STATUS Error\n");
    }
    return error_code;
}





