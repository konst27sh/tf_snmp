//
// Created by sheverdin on 4/16/25.
//

#ifndef TF_SNMP_SNMP_MODULE_H
#define TF_SNMP_SNMP_MODULE_H

#include <stdio.h>

#define UINT_MAX (65538)

typedef unsigned long oid;

typedef enum {
    NODE_TYPE_SCALAR,    // Скалярный объект (например, INTEGER)
    NODE_TYPE_TABLE,     // Таблица
    NODE_TYPE_ENTRY,     // Запись в таблице
    NODE_TYPE_COLUMN     // Колонка в таблице
}NodeType;

typedef struct TreeNode {
    char *name;                 // Имя узла (например, "poeStatus")
    oid *oid;                   // OID в числовом формате
    size_t oid_len;             // Длина OID
    NodeType type;              // Тип узла
    struct TreeNode *child;     // Первый дочерний узел
    struct TreeNode *next_peer; // Следующий узел того же уровня
}TreeNode;

#endif //TF_SNMP_SNMP_MODULE_H
