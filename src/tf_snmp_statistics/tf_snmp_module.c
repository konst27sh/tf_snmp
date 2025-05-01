//
// Created by sheverdin on 4/30/25.
//

#include <string.h>
#include <stdlib.h>

#include "tf_snmp_module.h"
#include "statusPSW/poeStatus/poeStatus_main.h"
#include "utils/debug.h"


Tree_NodeClass node_pool[MAX_NODES];
uint16_t node_count = 0;

uint16_t add_node(int oid_component, const char *name, NodeType type,
                  uint16_t parent_idx, void *data, HandlerFunc handler)
{
    if(node_count >= MAX_NODES)
    {
        printf("Error: Node pool overflow\n");
        return 0xFFFF;
    }

    Tree_NodeClass *node = &node_pool[node_count];
    node->staticTreeNode.oid_component = oid_component;
    node->staticTreeNode.name = name;
    node->staticTreeNode.type = type;
    node->staticTreeNode.data = data;
    node->staticTreeNode.parent_idx = parent_idx;
    node->staticTreeNode.first_child_idx = 0xFFFF;
    node->staticTreeNode.next_sibling_idx = 0xFFFF;
    node->handlerFunc = handler;

    // Связывание с родителем
    if (parent_idx != 0xFFFF && parent_idx < node_count)
        {
            Tree_NodeClass *parent = &node_pool[parent_idx];

            // Добавляем как первого ребенка или к sibling цепи
            if (parent->staticTreeNode.first_child_idx == 0xFFFF)
            {
                parent->staticTreeNode.first_child_idx = node_count;
            }
            else
            {
                uint16_t sibling = parent->staticTreeNode.first_child_idx;
                while (node_pool[sibling].staticTreeNode.next_sibling_idx != 0xFFFF)
                {
                    sibling = node_pool[sibling].staticTreeNode.next_sibling_idx;
                }
                node_pool[sibling].staticTreeNode.next_sibling_idx = node_count;
            }
    }
    LOG_DEV("Added node: %s (idx: %d, parent: %d)", name, node_count, parent_idx);
    return node_count++;
}

// Преобразует строку OID в массив чисел
int parse_oid(const char *oid_str, int *oid_buf)
{
    char *copy = strdup(oid_str);
    char *token = strtok(copy, ".");
    int count = 0;

    while(token && count < MAX_OID_LEN) {
        oid_buf[count++] = atoi(token);
        token = strtok(NULL, ".");
    }

    free(copy);
    return count;
}

uint16_t find_oid_node(const int *oid, int oid_len)
{
    uint16_t current_node = 0;
    LOG_DEBUG("oid_len = %d", oid_len);
    for (int depth = 0; depth < oid_len; depth++)
    {
        LOG_DEBUG("------------------------------------");
        Tree_NodeClass *node = &node_pool[current_node];
        uint16_t next_node = 0xFFFF;

        // Перебираем всех сиблингов текущего узла
        while (node != NULL)
        {
            LOG_DEBUG("depth = %d -- oid[depth] = %d -- oid_component = %d", depth, oid[depth], node->staticTreeNode.oid_component);
            if (node->staticTreeNode.oid_component == oid[depth])
            {
                LOG_DEBUG("if next_node -> %d", depth == oid_len - 1);
                LOG_DEBUG("child_idx = %d -- current_node = %d", node->staticTreeNode.first_child_idx, current_node);
                next_node = (depth == oid_len - 1) ? current_node : node->staticTreeNode.first_child_idx;
                LOG_DEBUG("next_node = %d", next_node);
                break;
            }
            LOG_DEBUG("next_sibling_idx = %d", node->staticTreeNode.next_sibling_idx);
            current_node = node->staticTreeNode.next_sibling_idx;
            node = (node->staticTreeNode.next_sibling_idx != 0xFFFF) ? &node_pool[node->staticTreeNode.next_sibling_idx] : NULL;

            if (node != NULL)
            {
                LOG_DEBUG("next_sibling_idx = %d", node->staticTreeNode.next_sibling_idx);
            }
            else
            {
                LOG_DEBUG("node != NULL");
            }
        }
        if (next_node == 0xFFFF)
            return 0xFFFF;
        LOG_DEBUG("next_node 2 = %d", next_node);
        current_node = next_node;
    }
    LOG_DEBUG("current_node = %d", current_node);
    return current_node;
}


uint16_t get_next_oid(uint16_t current_idx, const int *oid, int depth) {
    if (current_idx >= node_count) return 0;

    Tree_NodeClass *node = &node_pool[current_idx];

    // Поиск совпадения компонента
    while (node != NULL) {
        if (node->staticTreeNode.oid_component == oid[depth]) {
            if (depth == MAX_OID_DEPTH - 1 || node->staticTreeNode.first_child_idx == 0) {
                // Лист: следующий узел на том же уровне
                return node->staticTreeNode.next_sibling_idx;
            }
            // Рекурсия для дочерних узлов
            return get_next_oid(node->staticTreeNode.first_child_idx, oid, depth + 1);
        } else if (node->staticTreeNode.oid_component > oid[depth]) {
            // Найден больший компонент
            return current_idx;
        }
        node = (node->staticTreeNode.next_sibling_idx != 0) ? &node_pool[node->staticTreeNode.next_sibling_idx] : NULL;
    }
    return 0;
}

// Получение полного OID
void get_full_oid(uint16_t node_idx, char *buf) {
    int components[MAX_OID_DEPTH+1];
    int depth = 0;
    uint16_t current = node_idx;
    LOG_DEV("node_idx = %hu", node_idx);
    // Собираем компоненты от узла к корню
    while (current != 0xFFFF && depth <= MAX_OID_DEPTH) {
        components[depth] = node_pool[current].staticTreeNode.oid_component;
        LOG_DEV("depth = %d -- components = %d", depth, components[depth]);
        depth++;
        current = node_pool[current].staticTreeNode.parent_idx;
        LOG_DEV("current = %d", current);
    }

    // Формируем строку в обратном порядке (начиная с корня)
    buf[0] = '\0';
    for (int i = depth - 1; i >= 0; i--) {
        char num[MAX_OID_DEPTH+1];
        snprintf(num, sizeof(num), "%s%d", (i == depth - 1) ? "" : ".", components[i]);
        strcat(buf, num);
    }
}

// Отладочный вывод дерева
void print_tree_debug(uint16_t node_idx, int level)
{
    if(node_idx == 0xFFFF) return;
    Tree_NodeClass *node = &node_pool[node_idx];
    char oid_buf[256];
    get_full_oid(node_idx, oid_buf);

    for(int i = 0; i < level; i++) {
        printf("- ");
        fflush(stdout);
    }

    printf("%s - OID: %s, P: %d, C: %d, S: %d, INDEX: %d\n",
           node->staticTreeNode.name,
           oid_buf,
           node->staticTreeNode.parent_idx,
           node->staticTreeNode.first_child_idx,
           node->staticTreeNode.next_sibling_idx,
           node->staticTreeNode.oid_component
    );

    if(node->staticTreeNode.first_child_idx != 0xFFFF) {
        print_tree_debug(node->staticTreeNode.first_child_idx, level + 1);
    }
    if(node->staticTreeNode.next_sibling_idx != 0xFFFF) {
        print_tree_debug(node->staticTreeNode.next_sibling_idx, level);
    }
}

void print_node_info(uint16_t node_idx) {
    Tree_NodeClass *node = &node_pool[node_idx];
    char oid_buf[256];
    char oid[256];
    get_full_oid(node_idx, oid_buf);
    strcat(oid, oid_buf);
#if LOG_LEVEL > LOG_LEVEL_INFO
    printf("%s\n", oid);
    printf("%s\n", node_type_to_str(node->staticTreeNode.type));
#else
    LOG_INFO("%s", oid_buf);
        LOG_INFO("%s", node_type_to_str(node->staticTreeNode.type));
#endif

    if (node->staticTreeNode.type != NODE_INTERNAL)
    {
        if (node->staticTreeNode.type == NODE_LEAF_INT)
        {
            if (node->handlerFunc != NULL)
            {
                node->handlerFunc(&node->staticTreeNode);
            }
            else
            {
                printf("N/A (handler missing)\n");
            }
        }
        //else
        //{
        //    printf("%s\n", (char *)node->staticTreeNode.data);
        //}
    }
    else
    {
        printf("Value: N/A (internal node)\n");
    }
}

uint16_t get_next_node(uint16_t current_node, const int *oid, int depth, int oid_len) {
    if (current_node == 0xFFFF) return 0xFFFF;

    Tree_NodeClass *node = &node_pool[current_node];
    uint16_t next_node;

    // Если достигли конца запрошенного OID
    if (depth == oid_len) {
        // Возвращаем первый дочерний узел или следующего сиблинга
        if (node->staticTreeNode.first_child_idx != 0xFFFF) {
            return find_min_subtree(node->staticTreeNode.first_child_idx);
        }
        return node->staticTreeNode.next_sibling_idx;
    }

    // Поиск совпадающего компонента OID
    while (1) {
        if (node->staticTreeNode.oid_component == oid[depth]) {
            // Рекурсивный поиск в дочерних узлах
            if (node->staticTreeNode.first_child_idx != 0xFFFF) {
                next_node = get_next_node(node->staticTreeNode.first_child_idx, oid, depth + 1, oid_len);
                if (next_node != 0xFFFF) return next_node;
            }
            // Проверка сиблингов текущего уровня
            if (node->staticTreeNode.next_sibling_idx != 0xFFFF) {
                return node->staticTreeNode.next_sibling_idx;
            }
            break;
        } else if (node->staticTreeNode.oid_component > oid[depth]) {
            return find_min_subtree(current_node);
        }

        // Переход к следующему сиблингу
        if (node->staticTreeNode.next_sibling_idx == 0xFFFF) break;
        current_node = node->staticTreeNode.next_sibling_idx;
        node = &node_pool[current_node];
    }

    // Поиск следующей колонки через родителей
    uint16_t parent = node->staticTreeNode.parent_idx;
    while (parent != 0xFFFF) {
        if (node_pool[parent].staticTreeNode.next_sibling_idx != 0xFFFF) {
            return find_min_subtree(node_pool[parent].staticTreeNode.next_sibling_idx);
        }
        parent = node_pool[parent].staticTreeNode.parent_idx;
    }
    return 0xFFFF;
}

// Вспомогательная функция для поиска минимального узла в поддереве
uint16_t find_min_subtree(uint16_t start_node) {
    if(start_node == 0xFFFF) return 0xFFFF;

    uint16_t current = start_node;
    while(node_pool[current].staticTreeNode.first_child_idx != 0xFFFF) {
        current = node_pool[current].staticTreeNode.first_child_idx;
    }
    return current;
}

const char* node_type_to_str(NodeType type)
{
    switch (type) {
        case NODE_INTERNAL:
            return "INTERNAL";
        case NODE_LEAF_INT:
            return "INTEGER";
        case NODE_LEAF_STRING:
            return "STRING";
        default:
            return "UNKNOWN";
    }
}

uint16_t find_parent(uint16_t node_idx) {
    if(node_idx >= node_count) return 0xFFFF;
    return node_pool[node_idx].staticTreeNode.parent_idx;
}

// STATIC FUNCTION



