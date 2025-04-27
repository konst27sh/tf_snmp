//
// Created by sheverdin on 4/18/25.
//

#ifndef TF_SNMP_MAIN_SNMP_H
#define TF_SNMP_MAIN_SNMP_H

#include <stdio.h>
#include <stdint.h>

typedef unsigned long oid;


typedef enum {
    NODE_INTERNAL,
    NODE_LEAF_INT,
    NODE_LEAF_STRING
} NodeType;

typedef struct {
    int oid_component;
    NodeType type;
    void *data;
    uint16_t first_child_idx;
    uint16_t next_sibling_idx;
    uint16_t parent_idx;
    const char *name;
    //HandlerFunc handler; // Добавляем обработчик
} StaticTreeNode;

typedef void (*HandlerFunc)(StaticTreeNode *node);

typedef struct {
    StaticTreeNode staticTreeNode;
    HandlerFunc handlerFunc;
}Tree_NodeClass;




#define mu_assert(message, test) \
    do {                         \
        if (!(test))             \
            return message;      \
    } while (0)

#define mu_run_test(test)       \
    do {                        \
        char *message = test(); \
        tests_run++;            \
        if (message)            \
            return message;     \
    } while (0)

extern int tests_run;

#endif //TF_SNMP_MAIN_SNMP_H
