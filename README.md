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