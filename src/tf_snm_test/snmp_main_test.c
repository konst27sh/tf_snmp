//
// Created by sheverdin on 4/22/25.
//


// tests/test_poe.c
#include <stdio.h>
#include <string.h>
#include "poeStatus/poeStatus_main.h"
#include "utils.h"
#include "tf_snmp_module.h"

int tests_run = 0;

// Тест 1: Проверка корректности OID
static char *test_oid_parsing() {
    const char *oid_str = "1.3.6.1.4.1.42019.3.2.2.5.1.1.1.1";
    int oid[MAX_OID_LEN];
    int len = parse_oid(oid_str, oid);

    mu_assert("OID parsing failed", len == 15);
    mu_assert("First component mismatch", oid[0] == 1);
    mu_assert("Last component mismatch", oid[14] == 1);
    return 0;
}

// Тест 2: Проверка перехода к следующему OID
static char *test_get_next_oid() {
    int oid[] = {1,3,6,1,4,1,42019,3,2,2,5,1,1,1,1};
    uint16_t next_node = get_next_node(0, oid, 0, 15);
    char buf[256];
    get_full_oid(next_node, buf);
    LOG_DEBUG("buf = %s", buf);
    mu_assert("Next OID incorrect", strcmp(buf, ".1.3.6.1.4.1.42019.3.2.2.5.1.1.1.2") == 0);
    return 0;
}

// Запуск всех тестов
static char *all_tests() {
    mu_run_test(test_oid_parsing);
    mu_run_test(test_get_next_oid);
    return 0;
}

int main() {
    init_mib_tree(); // Инициализация дерева MIB

    char *result = all_tests();
    if (result != 0) {
        printf("TEST FAILED: %s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}