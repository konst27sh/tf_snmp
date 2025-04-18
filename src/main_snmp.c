#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syslog.h"

#include "snmp_module.h"
#include "poe_module.h"

//#include <net-snmp/net-snmp-config.h>
//#include <net-snmp/net-snmp-includes.h>
//#include <net-snmp/agent/net-snmp-agent-includes.h>


// Структура узла MIB
// OID для секции poeStatus: .1.3.6.1.4.1.42019.3.2.2.5
static oid poeStatus_oid[] = {1, 3, 6, 1, 4, 1, 42019, 3, 2, 2, 5};
static size_t poeStatus_oid_len = 11;

// OID для таблицы poeStatusTable: .1.3.6.1.4.1.42019.3.2.2.5.1
static oid poeStatusTable_oid[] = {1, 3, 6, 1, 4, 1, 42019, 3, 2, 2, 5, 1};
static size_t poeStatusTable_oid_len = 12;

// OID для записи poeStatusEntry: .1.3.6.1.4.1.42019.3.2.2.5.1.1
static oid poeStatusEntry_oid[] = {1, 3, 6, 1, 4, 1, 42019, 3, 2, 2, 5, 1, 1};
static size_t poeStatusEntry_oid_len = 13;

// OID для колонок:
// portPoeStatusIndex: .1.3.6.1.4.1.42019.3.2.2.5.1.1.1
static oid portPoeStatusIndex_oid[] = {1, 3, 6, 1, 4, 1, 42019, 3, 2, 2, 5, 1, 1, 1};
static size_t portPoeStatusIndex_oid_len = 14;

// portPoeStatusState: .1.3.6.1.4.1.42019.3.2.2.5.1.1.2
static oid portPoeStatusState_oid[] = {1, 3, 6, 1, 4, 1, 42019, 3, 2, 2, 5, 1, 1, 2};
static size_t portPoeStatusState_oid_len = 14;

// portPoeStatusPower: .1.3.6.1.4.1.42019.3.2.2.5.1.1.3
static oid portPoeStatusPower_oid[] = {1, 3, 6, 1, 4, 1, 42019, 3, 2, 2, 5, 1, 1, 3};
static size_t portPoeStatusPower_oid_len = 14;

void print_usage(const char *prog_name) {
    fprintf(stderr, "Usage: %s [-n|-g|-s] OID\n", prog_name);
}

int split_string_by_last_dot(const char* str, int N, char** str1, char** str2) {
    // Проверка входных данных
    if (str == NULL || str1 == NULL || str2 == NULL || N < 0) {
        return 1;
    }

    // Подсчет количества точек
    int dot_count = 0;
    const char* ptr = str;
    while (*ptr) {
        if (*ptr == '.') dot_count++;
        ptr++;
    }

    // Проверка условия
    if (dot_count != N) {
        return 2;
    }

    // Поиск последней точки
    const char* last_dot = strrchr(str, '.');
    if (last_dot == NULL || last_dot == str || *(last_dot + 1) == '\0') {
        return 3; // Нет точек или некорректный формат
    }

    // Выделение памяти для подстрок
    size_t len_part1 = last_dot - str;
    *str1 = (char*)malloc(len_part1 + 1);
    *str2 = (char*)malloc(strlen(last_dot + 1) + 1);

    if (*str1 == NULL || *str2 == NULL) {
        free(*str1);
        free(*str2);
        return 4;
    }

    // Копирование данных
    strncpy(*str1, str, len_part1);
    (*str1)[len_part1] = '\0';
    strcpy(*str2, last_dot + 1);

    return 0;
}

int str_to_oid(const char *input_str_oid, oid **output_oid, size_t *s_count) {
    size_t count = 0;
    if (input_str_oid == NULL || output_oid == NULL)
    {
        printf("error 1\n");
        return 1; // Некорректные аргументы
    }

    // Создание копии строки для безопасной обработки
    char *str = strdup(input_str_oid);
    if (str == NULL) {
        printf("error 2\n");
        return 2; // Ошибка выделения памяти
    }

    // Пропуск начальной точки (если есть)
    char *start = str;
    if (*start == '.') {
        start++;
    }

    // Подсчет количества элементов OID

    char *ptr = start;
    while ((ptr = strchr(ptr, '.')) != NULL) {
        count++;
        ptr++;
    }
    count++; // Учет последнего элемента

    // Выделение памяти под массив oid
    oid *oid_arr = (oid *)malloc( count * sizeof(oid));
    if (oid_arr == NULL) {
        free(str);
        printf("error 3\n");
        return 3;
    }

    // Разбиение строки на токены и преобразование в числа
    int i = 0;
    char *token = strtok(start, ".");
    //printf("num = ");
    while (token != NULL) {
        char *endptr;
        long num = strtol(token, &endptr, 10);
        //printf("%ld.",  num);
        // Проверка на ошибки преобразования
        if (*endptr != '\0' || num < 0 || num > UINT_MAX) {
            free(oid_arr);
            free(str);
            printf("error 4\n");
            return 4;
        }

        oid_arr[i++] = (oid)num;
        token = strtok(NULL, ".");
    }
    //printf("\n");
    free(str);
    *s_count = count;
    *output_oid = oid_arr;
    return 0;
}

TreeNode* create_node(const char *name, oid *oid_v, size_t oid_len, NodeType type)
{
    TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
    node->name = strdup(name);
    node->oid =  (oid*)malloc(oid_len * sizeof(oid));
    memcpy(node->oid, oid_v, oid_len * sizeof(oid));
    node->oid_len = oid_len;
    node->type = type;
    node->child = NULL;
    node->next_peer = NULL;
    return node;
}

// Построение дерева для poeStatus
TreeNode* build_poeStatus_tree() {
    // Корневой узел для poeStatus
    TreeNode *poeStatus = create_node("poeStatus", poeStatus_oid, poeStatus_oid_len, NODE_TYPE_SCALAR);

    // Узел таблицы poeStatusTable
    TreeNode *poeStatusTable = create_node("poeStatusTable", poeStatusTable_oid, poeStatusTable_oid_len, NODE_TYPE_TABLE);
    poeStatus->child = poeStatusTable;

    // Узел записи poeStatusEntry
    TreeNode *poeStatusEntry = create_node("poeStatusEntry", poeStatusEntry_oid, poeStatusEntry_oid_len, NODE_TYPE_ENTRY);
    poeStatusTable->child = poeStatusEntry;

    // Колонки таблицы
    TreeNode *col1 = create_node("portPoeStatusIndex", portPoeStatusIndex_oid, portPoeStatusIndex_oid_len, NODE_TYPE_COLUMN);
    TreeNode *col2 = create_node("portPoeStatusState", portPoeStatusState_oid, portPoeStatusState_oid_len, NODE_TYPE_COLUMN);
    TreeNode *col3 = create_node("portPoeStatusPower", portPoeStatusPower_oid, portPoeStatusPower_oid_len, NODE_TYPE_COLUMN);

    // Связывание колонок
    poeStatusEntry->child = col1;
    col1->next_peer = col2;
    col2->next_peer = col3;

    return poeStatus;
}

TreeNode* find_next_oid(TreeNode *root, oid *requested_oid, size_t oid_len, int request_type) {
    TreeNode *current = root;
    TreeNode *next = NULL;

    // Рекурсивный обход дерева
    while (current != NULL) {
        // Сравнение OID

        int cmp = memcmp(current->oid, requested_oid, oid_len * sizeof(oid));

        //printf("current : ");
        //for (size_t i = 0; i < current->oid_len; i++) {
        //    printf(".%lu", current->oid[i]);
        //}
        //printf(" (%s)\n", current->name);

        //printf("requested_oid 1: ");
        //for (size_t i = 0; i < oid_len; i++) {
        //    printf(".%lu", requested_oid[i]);
        //}
        //printf("\n");
        //printf("cmp = %d\n", cmp);
        if (cmp == 0)
        {
            if (request_type == 1)
            {
                next = current;
                return next;
            }
        }
        else if (cmp > 0)
        {
            //printf("cmp > 0\n");
            if (next == NULL || memcmp(current->oid, next->oid, current->oid_len * sizeof(oid)) < 0) {
                //printf("next == NULL\n");
                next = current;
                //printf(" next->oid_len = %zu\n",  next->oid_len);
                //printf("next 1: ");
                //for (size_t i = 0; i < next->oid_len; i++) {
                //    printf(".%lu", next->oid[i]);
                //}
                //printf(" (%s) - type == %d\n", next->name , next->type);
            }
        }

        // Проверка дочерних узлов
        if (current->child != NULL)
        {
            //printf("current->child != NULL\n");
            TreeNode *child_next = find_next_oid(current->child, requested_oid, oid_len, request_type);
            if (child_next != NULL)
            {
                //printf("child_next != NULL\n");
                if (next == NULL || memcmp(child_next->oid, next->oid, child_next->oid_len * sizeof(oid)) < 0) {
                    //printf("next == NULL\n");
                    next = child_next;
                    //printf(" next->oid_len = %zu\n",  next->oid_len);
                    //printf("next 2: ");
                    //for (size_t i = 0; i < next->oid_len; i++) {
                    //    printf(".%lu", next->oid[i]);
                    //}
                    //printf(" (%s)\n", next->name);
                }
            }
        }
        current = current->next_peer;
    }
    return next;
}

void free_tree(TreeNode *root) {
    if (root == NULL) return;

    // Рекурсивное удаление дочерних узлов
    free_tree(root->child);

    // Рекурсивное удаление соседних узлов
    free_tree(root->next_peer);

    // Освобождение ресурсов текущего узла
    free(root->name);
    free(root->oid);
    free(root);
}

int main(int argc, char **argv) {

    openlog("tf_snmp", 0, LOG_USER);
    syslog(LOG_ALERT, "tf_snmp started.... ");

    if (argc != 3) {
        syslog(LOG_ALERT, "no arg");
        print_usage(argv[0]);
        return 1;
    }

    char *action  = argv[1];
    char *input_oid_str = argv[2];

    syslog(LOG_ALERT, "input oid = %s" , input_oid_str);
    syslog(LOG_ALERT, "action = %s" , action);

    //printf("Action: %s\n", action);
    //printf("Input OID: %s\n", oid_str);
    TreeNode *next_node = NULL;
    char* part1         = NULL;
    char* part2         = NULL;

    // Построение дерева
    TreeNode *poeStatus_tree = build_poeStatus_tree();
    oid *oid_arr = NULL;

    size_t input_oid_len = 0;

    int res_input_oid = str_to_oid(input_oid_str, &oid_arr, &input_oid_len);
    if (res_input_oid > 0)
    {
        printf("%s\n", input_oid_str);
        printf("INTEGER\n");
        printf("%s\n", "555");
        //printf("ERROR  = %d\n", res_input_oid);
        return 1;
    }
    //printf("input_oid_len = %zu\n", input_oid_len);
    syslog(LOG_ALERT, "input_oid_len = %zu" , input_oid_len);
    if (input_oid_len == 15)
    {
        int res_split = split_string_by_last_dot(input_oid_str, 15, &part1, &part2);
        if (res_split > 0 ) {
            printf("ERROR split = %d\n", res_split);
        }

        //printf("Part 1: %s\n", part1); // abc.def.123
        //printf("Part 2: %s\n", part2); // 456
        oid_arr = NULL;
        input_oid_len = 0;
        res_input_oid = str_to_oid(part1, &oid_arr, &input_oid_len);
        if (res_input_oid > 0)
        {
            //printf("ERROR  = %d\n", res_input_oid);
            printf("%s\n", input_oid_str);
            printf("INTEGER\n");
            printf("%s\n", "777");
            return 1;
        }
    }
    // Поиск следующего OID
    if (strcmp(action, "-n") == 0)
    {
        next_node = find_next_oid(poeStatus_tree, oid_arr, input_oid_len, 2);
    }
    else if (strcmp(action, "-g") == 0)
    {
        next_node = find_next_oid(poeStatus_tree, oid_arr, input_oid_len, 1);
        if (next_node != NULL)
        {
            int num = (int)strtol(part2, NULL, 10);
            //printf("last num  = %ld\n", num);
            char poeStatus_data[8];
            int poeStatus_res = snmp_getPoeStatus(next_node, num, poeStatus_data);
            if (poeStatus_res == 0)
            {
                printf("%s\n", input_oid_str);
                printf("INTEGER\n");
                printf("%s\n", poeStatus_data);
                syslog(LOG_ALERT, "input oid = %s" , input_oid_str);
                syslog(LOG_ALERT, "INTEGER");
                syslog(LOG_ALERT, "poeStatus_data = %s" , poeStatus_data);
            }
            else
            {
                printf("error:get poe status\n");
            }
        }
    }

    if (next_node != NULL) {
        //printf("Next OID: ");
        //for (size_t i = 0; i < next_node->oid_len; i++) {
        //    printf(".%lu", next_node->oid[i]);
        //}
        //printf(" (%s)\n", next_node->name);
    } else {
        printf("No next OID found.\n");
    }
    free(part1);
    free(part2);
    free_tree(poeStatus_tree);
    closelog();
    return 0;
}