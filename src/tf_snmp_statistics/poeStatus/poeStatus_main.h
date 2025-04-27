//
// Created by sheverdin on 4/21/25.
//

#ifndef TF_SNMP_POESTATUS_MAIN_H
#define TF_SNMP_POESTATUS_MAIN_H

#include "../tf_snmp_module.h"

#define POE_PARAMS 3      // Index, State, Power
#define POE_PORTS 8       // Портов на параметр
#define POE_NODES (12 + 1 + POE_PARAMS * (1 + POE_PORTS)) // 12+1+3*9=40

// С запасом для других объектов:
#define MAX_NODES (POE_NODES * 2) // 80

#define MAX_OID_LEN 64
#define MAX_OID_DEPTH 14


uint16_t add_node(int oid_component, const char *name, NodeType type,
                  uint16_t parent_idx, void *data, HandlerFunc handler);
void init_mib_tree();
void init_static_tree();
int parse_oid(const char *oid_str, int *oid_buf);
const char* node_type_to_str(NodeType type);
uint16_t find_parent(uint16_t node_idx);
void print_node_info(uint16_t node_idx);
void print_tree_debug(uint16_t node_idx, int level);
uint16_t get_next_node(uint16_t current_node, const int *oid, int depth, int oid_len);
uint16_t find_min_subtree(uint16_t start_node);
void get_full_oid(uint16_t node_idx, char *buf);
uint16_t find_oid_node(const int *oid, int oid_len);

#endif //TF_SNMP_POESTATUS_MAIN_H
