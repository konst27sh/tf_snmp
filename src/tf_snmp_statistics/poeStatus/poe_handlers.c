//
// Created by sheverdin on 4/22/25.
//

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "../utils.h"
#include "poe_handlers.h"

static int get_poeIinfo(uint16_t port, char *res);

void get_poeIndex(StaticTreeNode *node)
{
    #if LOG_LEVEL > LOG_LEVEL_INFO
        printf("%d\n", node->oid_component);
    #endif

    LOG_INFO("index = %d -- index = %hu", node->oid_component, node->oid_component);
}

void get_poeStatus(StaticTreeNode *node)
{
    json_t *root = NULL;
    char poeInfo[1024];
    const char *status_v = NULL;

    int error_code = get_poeIinfo(node->oid_component, poeInfo);

    if (!error_code)
    {
        root = getData_formJson(poeInfo);
        if (root != NULL)
        {
            json_t *values = json_object_get(root, "status");
            if (values != NULL)
            {
                status_v = json_string_value(values);
                LOG_DEBUG("status_v = %s", status_v);
            }
        }
    }
    #if LOG_LEVEL > LOG_LEVEL_INFO
        printf("%s\n", status_v);
    #endif
    json_decref(root);
}

void get_poePower(StaticTreeNode *node)
{
    char power_res[8];
    json_t *root = NULL;
    char poeInfo[1024];
    int error_code = get_poeIinfo(node->oid_component, poeInfo);
    LOG_WARN("poeInfo:");
    LOG_WARN("%s", poeInfo);
    if (!error_code)
    {
        root = getData_formJson(poeInfo);
        if (root != NULL)
        {
            json_t *values = json_object_get(root, "power");
            if (values != NULL)
            {
                int power_v = (int) json_number_value(values);
                char power_str[8];
                uint16_t len = int_to_string(power_v, power_str);
                LOG_WARN("power_v = %d", power_v);
                LOG_WARN("power_str = %s", power_str);
                if (len > 3)
                    LOG_FATAL("poe power not corrected");
                strncpy(power_res, power_str, len);
            }
        }
        #if LOG_LEVEL > LOG_LEVEL_INFO
            printf("%s\n", power_res);
        #endif
    }
}

static int get_poeIinfo(uint16_t port, char *res)
{
    if (port < 1 || port > 8)
        return 1;

    char command[128];
    snprintf(command, sizeof(command), "ubus call poe portInfo '{\"port\":\"lan%d\"}'", port);
    FILE* fp = popen(command, "r");
    if (!fp)
        return 2;

    size_t len = fread(res, 1, 1022, fp);
    res[1023] = '\0';
    pclose(fp);
    return 0;
}


