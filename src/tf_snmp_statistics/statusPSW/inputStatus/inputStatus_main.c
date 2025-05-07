//
// Created by sheverdin on 5/5/25.
//

#include "inputStatus_main.h"
#include "../../utils/debug.h"
#include "../../utils/utils.h"

#define inputType_size  (3)
#define inputState_size (3)
#define inputAlarm_size (4)

const char *inputState_columns[INPUT_STATUS_COL] = {
    "inputIndex",
    "inputType",
    "autoRstDstIP",
    "inputState",
};

const char *inputState_index[INPUT_STATUS_COL] = {
    "sensor1",
    "sensor2",
    "relay",
};

const char *inputType_list[inputType_size] =
{
    "not_defined",
    "build-in",
    "pic",
};

const char *inputAlarm_list[inputAlarm_size] =
{
    "not_defined",
    "short",
    "open",
    "any"
};


static int get_inputData(const char* section, const char* option, char *res);

void get_inputIndex(StaticTreeNode *node)
{
    LOG_DEBUG("get_inputIndex");
    LOG_DEBUG("index = %d", node->oid_component);
    printf("%d\n", node->oid_component);
}

void get_inputType(StaticTreeNode *node)
{
    LOG_DEBUG("get_inputType");
    uint8_t inputType = 0;
    char data[MAX_BUFFER_SIZE] = {0};
    char result[MAX_BUFFER_SIZE] = {0};
    get_inputData(node->name, "type", data);
    get_string_data(data, result, "value");
    LOG_DEBUG("result = %s", result);
    for (int i = 1; i < inputType_size; i++)
    {
        if (strcmp(inputType_list[i], result) == 0 )
        {
            inputType = i;
            break;
        }
    }
    printf("%d\n", inputType);
}

void get_inputState(StaticTreeNode *node)
{
    LOG_DEBUG("get_inputState");
    char data[MAX_BUFFER_SIZE] = {0};
    char result[MAX_BUFFER_SIZE] = {0};
    get_i2c_Data(node->name, data);
    get_string_data(data, result, node->name);
    LOG_DEBUG("result = %s", result);
    printf("%s\n", result);
}

void get_inputAlarm(StaticTreeNode *node)
{
    LOG_DEBUG("get_inputAlarm");
    uint8_t inputAlarm = 0;
    char data[MAX_BUFFER_SIZE] = {0};
    char result[MAX_BUFFER_SIZE] = {0};
    get_inputData(node->name, "alarm_state", data);
    get_string_data(data, result, "value");
    LOG_DEBUG("result = %s", result);
    for (int i = 1; i < inputAlarm_size; i++)
    {
        if (strcmp(inputAlarm_list[i], result) == 0 )
        {
            inputAlarm = i;
            break;
        }
    }
    printf("%d\n", inputAlarm);
}

uint16_t init_mib_inputStatus(uint16_t parent_index)
{
    uint16_t inputStatus = add_node(2, "inputStatus", NODE_INTERNAL, parent_index, NULL, NULL);

    uint16_t inputStatus_table = add_node(1, "inputStatusTable", NODE_INTERNAL, inputStatus, NULL, NULL);
    uint16_t inputStatus_entry = add_node(1, "inputStatusEntry", NODE_INTERNAL, inputStatus_table, NULL, NULL);

    for(int col = 0; col < INPUT_STATUS_COL; col++)
    {
        uint16_t col_node = add_node(col+1, inputState_columns[col], NODE_INTERNAL, inputStatus_entry, NULL, NULL);

        for(int port = 0; port < INPUT_STATUS_PORTS; port++)
        {
            void *data = NULL;
            HandlerFunc inputStatusHandlers[] = {
                get_inputIndex,
                get_inputType,
                get_inputState,
                get_inputAlarm,
            };
            add_node(port + 1, inputState_index[port], NODE_LEAF_INT, col_node, data, inputStatusHandlers[col]);
        }
    }
    return inputStatus;
}

static int get_inputData(const char* section, const char* option, char *res)
{
    char command[128];
    snprintf(command, sizeof(command), "ubus call uci get '{\"config\":\"tfortis_io\", \"section\":\"%s\", \"option\":\"%s\"}'" , section, option);
    LOG_DEBUG("command = %s", command);
    FILE* fp = popen(command, "r");
    if (!fp)
        return 2;

    size_t len = fread(res, 1, 256, fp);
    res[len] = '\0';
    pclose(fp);
    return 0;
}

