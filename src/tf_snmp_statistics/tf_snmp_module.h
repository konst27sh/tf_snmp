//
// Created by sheverdin on 4/18/25.
//

#ifndef TF_SNMP_MAIN_SNMP_H
#define TF_SNMP_MAIN_SNMP_H

#include <stdio.h>
#include <stdint.h>


#define POE_PARAMS 3      // Index, State, Power
#define POE_PORTS 8       // Портов на параметр
#define POE_NODES (12 + 1 + POE_PARAMS * (1 + POE_PORTS)) // 12+1+3*9=40

#define MAX_NODES (500) //(POE_NODES * 2) // 80
#define MAX_OID_LEN 64
#define MAX_OID_DEPTH 15

typedef unsigned long oid;

typedef enum {
    NODE_INTERNAL,
    NODE_LEAF_INT,
    NODE_LEAF_STRING,
    NODE_LEAF_IP,
    OCTET_STRING
}NodeType;

typedef struct {
    int oid_component;
    NodeType type;
    void *data;
    uint16_t first_child_idx;
    uint16_t next_sibling_idx;
    uint16_t parent_idx;
    const char *name;
} StaticTreeNode;

typedef void (*HandlerFunc)(StaticTreeNode *node);

typedef struct {
    StaticTreeNode staticTreeNode;
    HandlerFunc handlerFunc;
}Tree_NodeClass;

uint16_t add_node(int oid_component, const char *name, NodeType type, uint16_t parent_idx, void *data, HandlerFunc handler);
int parse_oid(const char *oid_str, int *oid_buf);
const char* node_type_to_str(NodeType type);
uint16_t find_parent(uint16_t node_idx);
void print_node_info(uint16_t node_idx);
void print_tree_debug(uint16_t node_idx, int level);
uint16_t get_next_node(uint16_t current_node, const int *oid, int depth, int oid_len);
uint16_t find_min_subtree(uint16_t start_node);
void get_full_oid(uint16_t node_idx, char *buf);
uint16_t find_oid_node(const int *oid, int oid_len);


extern int tests_run;

#endif //TF_SNMP_MAIN_SNMP_H
