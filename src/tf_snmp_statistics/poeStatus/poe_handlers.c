//
// Created by sheverdin on 4/22/25.
//

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "../utils.h"
#include "poe_handlers.h"

static int get_poe_info(uint16_t port, const char* type, char *data);

void get_poeIndex(StaticTreeNode *node)
{
    #if LOG_LEVEL > LOG_LEVEL_INFO
        printf("%d\n", node->oid_component);
    #endif
    LOG_INFO("index = %d -- index = %hu", node->oid_component, node->oid_component);
}

void get_poeStatus(StaticTreeNode *node)
{
    char status_res[24];
    int error_code = get_poe_info(node->oid_component, "status", status_res);

    LOG_DEV("error_code = %d", error_code);
    LOG_INFO("index = %d -- status = %s", node->oid_component, status_res);

    #if LOG_LEVEL > LOG_LEVEL_INFO
        printf("%s\n", status_res);
    #endif
}

void get_poePower(StaticTreeNode *node)
{
    char power_res[8];
    int error_code = get_poe_info(node->oid_component, "power", power_res);
    LOG_DEV("error_code = %d", error_code);
    LOG_INFO("index = %d -- power = %s", node->oid_component, power_res);
}

static int get_poe_info(uint16_t port, const char* type, char *data)
{
    if (port < 1 || port > 8) return 1;
    if (strcmp(type, "status") != 0 && strcmp(type, "power") != 0) return 2;

    // Формирование команды
    char command[128];
    snprintf(command, sizeof(command), "ubus call poe portInfo '{\"port\":\"lan%d\"}'", port);

    // Выполнение команды
    FILE* fp = popen(command, "r");
    if (!fp) return 3;

    // Чтение вывода
    char output[1024];
    size_t len = fread(output, 1, sizeof(output) - 1, fp);
    output[len] = '\0';
    pclose(fp);
    int res = getData_formJson(output, type, data);
    LOG_DEBUG("res = %d", res);
    LOG_DEBUG("data = %s", data);
    #if LOG_LEVEL > LOG_LEVEL_INFO
        printf("%s\n", data);
    #endif
    return 0;
}


