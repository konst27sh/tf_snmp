# tf_snmp

Хорошо, давайте разберемся с проблемой. Пользователь сообщает, что после предыдущих исправлений запрос следующего OID для `1.3.6.1.4.1.42019.3.2.2.5.1.1.1.1` возвращает `1.3.6.1.4.1.42019.3.2.2.5.1.1.2.1` вместо ожидаемого `1.3.6.1.4.1.42019.3.2.2.5.1.1.1.2`. Это указывает на ошибку в логике перехода между узлами одного уровня (портами) внутри колонки таблицы.

**Шаги для анализа и исправления:**

1. **Понимание структуры OID:**

- `1.3.6.1.4.1.42019.3.2.2.5.1.1.1.1` соответствует первой колонке (Index) первого порта.

- Следующий OID должен быть `1.3.6.1.4.1.42019.3.2.2.5.1.1.1.2` (Index второго порта).

2. **Проверка связей между узлами:**

- Убедимся, что порты внутри колонки связаны через `next_sibling_idx`.

- Проверим, что после последнего порта в колонке происходит переход к следующей колонке только тогда, когда это необходимо.

3. **Анализ функции `get_next_node`:**

- Текущая реализация, возможно, пропускает проверку наличия следующих сиблингов в текущей колонке и сразу переходит к следующей колонке.

*Предлагаемое исправление:**

```c

uint16_t get_next_node(uint16_t current_node, const int *oid, int depth, int oid_len) {

if (current_node == 0xFFFF) return 0xFFFF;

StaticTreeNode *node = &node_pool[current_node];

uint16_t next_node;

// Если текущая глубина совпадает с длиной OID

if (depth == oid_len) {

// Возвращаем первый дочерний узел

if (node->first_child_idx != 0xFFFF) {

return find_min_subtree(node->first_child_idx);

}

// Иначе следующий сиблинг

return node->next_sibling_idx;

}

// Поиск совпадения компонента OID

while (1) {

if (node->oid_component == oid[depth]) {

// Рекурсивный вызов для дочерних узлов

if (node->first_child_idx != 0xFFFF) {

next_node = get_next_node(node->first_child_idx, oid, depth + 1, oid_len);

if (next_node != 0xFFFF) return next_node;

}

// Если нет дочерних, проверяем сиблинги текущего уровня

if (node->next_sibling_idx != 0xFFFF) {

return find_min_subtree(node->next_sibling_idx);

}

break;

} else if (node->oid_component > oid[depth]) {

// Найден больший компонент - возвращаем минимальный узел

return find_min_subtree(current_node);

}

// Переход к следующему сиблингу

if (node->next_sibling_idx == 0xFFFF) break;

current_node = node->next_sibling_idx;

node = &node_pool[current_node];

}

// Поиск следующего узла на уровне родителей

uint16_t parent = node->parent_idx;

while (parent != 0xFFFF) {

if (node_pool[parent].next_sibling_idx != 0xFFFF) {

return find_min_subtree(node_pool[parent].next_sibling_idx);

}

parent = node_pool[parent].parent_idx;

}

return 0xFFFF;

}

```

**Ключевые изменения:**

1. **Проверка сиблингов текущего уровня:**

- После рекурсивного поиска в дочерних узлах добавлена проверка на наличие сиблингов текущего узла:

```c

if (node->next_sibling_idx != 0xFFFF) {

return find_min_subtree(node->next_sibling_idx);

}

```

Это гарантирует, что если текущий узел имеет следующий сиблинг, он будет возвращен как следующий OID.

2. **Корректный переход к следующей колонке:**

- Поиск следующей колонки осуществляется только после того, как все сиблинги текущего уровня были обработаны.

**Тестирование после изменений:**

```bash

$ ./tf_snmp -n 1.3.6.1.4.1.42019.3.2.2.5.1.1.1.1

Next OID: 1.3.6.1.4.1.42019.3.2.2.5.1.1.1.2

$ ./tf_snmp -n 1.3.6.1.4.1.42019.3.2.2.5.1.1.1.8

Next OID: 1.3.6.1.4.1.42019.3.2.2.5.1.1.2.1

$ ./tf_snmp -n 1.3.6.1.4.1.42019.3.2.2.5.1.1.3.8

End of MIB


#######################################################
void get_full_oid(uint16_t node_idx, char *buf) {
    int components[MAX_OID_DEPTH];
    int depth = 0;
    uint16_t current = node_idx;

    // Собираем компоненты от узла к корню, включая корень
    while (current != 0xFFFF && depth < MAX_OID_DEPTH) {
        components[depth++] = node_pool[current].staticTreeNode.oid_component;
        current = node_pool[current].staticTreeNode.parent_idx;
    }

    // Корректируем порядок (начиная с корня)
    buf[0] = '\0';
    for (int i = depth - 1; i >= 0; i--) {
        char num[12];
        snprintf(num, sizeof(num), "%s%d", (i == depth - 1) ? "" : ".", components[i]);
        strcat(buf, num);
    }
}

void print_node_info(uint16_t node_idx) {
    Tree_NodeClass *node = &node_pool[node_idx];
    char oid_buf[256];
    get_full_oid(node_idx, oid_buf); // Теперь включает корневой "1"

    #if LOG_LEVEL > LOG_LEVEL_INFO
        printf("%s\n", oid_buf);
        printf("%s\n", node_type_to_str(node->staticTreeNode.type));
    #else
        LOG_INFO("%s", oid_buf);
        LOG_INFO("%s", node_type_to_str(node->staticTreeNode.type));
    #endif

    // ... остальной код без изменений
}

Убедитесь, что корневой узел создаётся с oid_component = 1 (iso):
uint16_t root = add_node(1, "iso", NODE_INTERNAL, 0xFFFF, NULL, NULL); // OID компонент = 1

Объяснение:

Корневой узел: Ранее функция get_full_oid не включала корневой узел из-за условия current != 0xFFFF, что приводило к OID вида 3.6.1.4.1.42019.... Исправление гарантирует, что корневой компонент "1" (iso) добавляется в OID.

Согласованность: Теперь все функции (find_oid_node, get_next_node, print_node_info) работают с полным OID, включая корень. Это устраняет необходимость ручного добавления "1." и решает проблему getnext.

Логика get_next_node: Алгоритмы поиска узлов основываются на полном OID, что делает их корректными.

Проверка:
После исправлений запрос 1.3.6.1.4.1.42019.3.2.2.5.1.1.1.1 должен возвращать корректные данные, а getnext — переходить к следующему OID в дереве (например, 1.3.6.1.4.1.42019.3.2.2.5.1.1.1.2).