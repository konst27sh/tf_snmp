//
// Created by sheverdin on 5/5/25.
//

#include "sfpStatus.h"
#include "../../utils/debug.h"

#define SFP_PORTS 2

const char *sfp_columns[SFP_STATUS_COL] =
{
    "portSfpIndex",
    "portSfpPresent",
    "portSfpSignalDetect",
    "portSfpVendor",
    "portSfpOui",
    "portSfpPartNumber",
    "portSfpRevision",
    "portSfpTemperature",
    "portSfpVoltage",
    "portSfpBiasCurrent",
    "portSfpTxOutPower",
    "portSfpTxOutPowerDb",
    "portSfpRxOutPower",
    "portSfpRxOutPowerDb",
};

void get_portSfpIndex(StaticTreeNode *node)
{
    LOG_FATAL("get_portSfpIndex");
}

void get_portSfpPresent(StaticTreeNode *node)
{
    LOG_FATAL("get_portSfpPresent");
}

void get_portSfpSignalDetect(StaticTreeNode *node)
{
    LOG_FATAL("get_portSfpSignalDetect");
}

void get_portSfpVendor(StaticTreeNode *node)
{
    LOG_FATAL("get_portSfpVendor");
}

void get_portSfpOui(StaticTreeNode *node)
{
    LOG_FATAL("get_portSfpOui");
}

void get_portSfpPartNumber(StaticTreeNode *node)
{
    LOG_FATAL("get_portSfpPartNumber");
}

void get_portSfpRevision(StaticTreeNode *node)
{
    LOG_FATAL("get_portSfpRevision");
}

void get_portSfpTemperature(StaticTreeNode *node)
{
    LOG_FATAL("get_portSfpTemperature");
}

void get_portSfpVoltage(StaticTreeNode *node)
{
    LOG_FATAL("get_portSfpVoltage");
}

void get_portSfpBiasCurrent(StaticTreeNode *node)
{
    LOG_FATAL("get_portSfpBiasCurrent");
}

void get_portSfpTxOutPower(StaticTreeNode *node)
{
    LOG_FATAL("get_portSfpTxOutPower");
}

void get_portSfpTxOutPowerDb(StaticTreeNode *node)
{
    LOG_FATAL("get_portSfpTxOutPowerDb");
}

void get_portSfpRxOutPower(StaticTreeNode *node)
{
    LOG_FATAL("get_portSfpRxOutPower");
}

void get_portSfpRxOutPowerDb(StaticTreeNode *node)
{
    LOG_FATAL("get_portSfpRxOutPowerDb");
}

uint16_t init_mib_sfpStatus(uint16_t parent_index)
{
    uint16_t sfpStatus = add_node(7, "sfpStatus", NODE_INTERNAL, parent_index, NULL, NULL);

    uint16_t sfpStatus_table = add_node(1, "sfpStatusTable", NODE_INTERNAL, sfpStatus, NULL, NULL);
    uint16_t sfpStatus_entry = add_node(1, "autoRestartEntry", NODE_INTERNAL, sfpStatus_table, NULL, NULL);

    for(int col = 0; col < SFP_STATUS_COL; col++)
    {
        uint16_t col_node = add_node(col+1, sfp_columns[col], NODE_INTERNAL, sfpStatus_entry, NULL, NULL);

        // Добавляем порты
        for(int port = 1; port <= SFP_PORTS; port++)
        {
            void *data = NULL;
            HandlerFunc sfpHandlers[] = {
                get_portSfpIndex,
                get_portSfpPresent,
                get_portSfpSignalDetect,
                get_portSfpVendor,
                get_portSfpOui,
                get_portSfpPartNumber,
                get_portSfpRevision,
                get_portSfpTemperature,
                get_portSfpVoltage,
                get_portSfpBiasCurrent,
                get_portSfpTxOutPower,
                get_portSfpTxOutPowerDb,
                get_portSfpRxOutPower,
                get_portSfpRxOutPowerDb,
            };
            add_node(port, "sfp_portEntry", NODE_LEAF_INT, col_node, data, sfpHandlers[col]);
        }
    }
    return sfpStatus;
}