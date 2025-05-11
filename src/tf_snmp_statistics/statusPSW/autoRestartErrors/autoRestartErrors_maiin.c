//
// Created by sheverdin on 5/5/25.
//

#include "autoRestartErrors_maiin.h"
#include "../../utils/debug.h"
#include "../../utils/utils.h"

const char *arErr_columns[AUTO_RESTART_ERR_COL] =
{
    "arPortIndex",
    "arPortStatus",
};

const char *arPortStatus[5] =
{
    NULL,
    "normal",
    "noLink",
    "noPing",
    "lowSpeed"
};

typedef enum
{
    normal      = 1,
    noLink      = 2,
    noPing      = 3,
    lowSpeed    = 4,
    testDisable = 5,
}AR_ERROR_RES;

typedef enum
{

    ERR_TEST_LINK               = 0x00004,
    ERR_TEST_PING               = 0x00008,
    ERR_TEST_SPEED              = 0x00010,
    ERR_TIME_ALARM              = 0x00020,
    ERR_TEST_DISABLE            = 0x00040,
    ERR_POE_DOWN                = 0x00080,
    ERR_IP_NOT_VALID            = 0x01000,
    ERR_PoE_DISABLE             = 0x02000,
    ERR_OK                      = 0x10000,
    ERR_PORT_SHUTDOWN           = 0x20000,
    ERR_MANUAL_REBOOT           = 0x40000,
    ERR_REBOOTING               = 0x80000
}error_code_t;

void get_arPortIndex(StaticTreeNode *node)
{
    printf("%d\n", node->oid_component);
}

void get_arPortStatus(StaticTreeNode *node)
{
    AR_ERROR_RES ar_errorRes = normal;
    char data[MAX_BUFFER_SIZE] = {0};
    char result[MAX_BUFFER_SIZE] = {0};
    char command[128];
    snprintf(command, sizeof(command), "tf_autorestart_ctrl status %d", node->oid_component);
    get_ubus_Data(command, data, MAX_BUFFER_SIZE);
    get_json_array(data, "port",  "error_Code", result);
    uint8_t ar_errorCode = strtol(result, NULL, 10);
    if (ar_errorCode & ERR_TEST_PING)
        ar_errorRes = noPing;
    else if (ar_errorCode & ERR_TEST_SPEED)
        ar_errorRes = lowSpeed;
    else if (ar_errorCode & ERR_TEST_LINK)
        ar_errorRes = noLink;
    else if (ar_errorCode & ERR_TEST_DISABLE)
        ar_errorRes = testDisable;
    printf("%d\n", ar_errorRes);
}


uint16_t init_mib_autoRestartErr(uint16_t parent_index)
{
    uint16_t specialStatus = add_node(6, "specialStatus", NODE_INTERNAL, parent_index, NULL, NULL);
    uint16_t autoRestartErrors = add_node(1, "autoRestartErrors", NODE_INTERNAL, specialStatus, NULL, NULL);

    uint16_t autoRestartErrors_table = add_node(1, "autoRestartTable", NODE_INTERNAL, autoRestartErrors, NULL, NULL);
    uint16_t autoRestartErrors_entry = add_node(1, "autoRestartEntry", NODE_INTERNAL, autoRestartErrors_table, NULL, NULL);

    for(int col = 0; col < AUTO_RESTART_ERR_COL; col++)
    {
        uint16_t col_node = add_node(col+1, arErr_columns[col], NODE_INTERNAL, autoRestartErrors_entry, NULL, NULL);

        for(int port = 1; port <= POE_PORTS; port++)
        {
            void *data = NULL;
            HandlerFunc autoRestartErrorsHandlers[] = {
                get_arPortIndex,
                get_arPortStatus,
            };
            add_node(port, "arErr_portEntry", NODE_LEAF_INT, col_node, data, autoRestartErrorsHandlers[col]);
        }
    }
    return specialStatus;
}

