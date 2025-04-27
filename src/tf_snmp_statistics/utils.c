//
// Created by sheverdin on 4/18/25.
//

#include <stdio.h>
#include "utils.h"
#include <jansson.h>
#include <string.h>

static char* int_to_string(int num);

int getData_formJson(char* json_data, const char *option, char *data)
{
    json_t *root;
    json_error_t error;
    const char *value = NULL;
    root = json_loads(json_data, 0, &error);
    if (!root) {
        LOG_ERROR("error: on line %d: %s", error.line, error.text);
        strcpy(data, "NULL");
        return 1;
    }
    json_t *values = json_object_get(root, option);
    if (values == NULL)
    {
        strcpy(data, "NULL");
        return 2;
    }
    if (strcmp(option, "status") == 0)
    {
        const char *status_v =  json_string_value(values);
        LOG_DEBUG("status_v = %s", status_v);
        strcpy(data, status_v);
    }
    else if (strcmp(option, "power") == 0)
    {
        int power_v = (int) json_number_value(values);
        char* power_str = int_to_string(power_v);
        LOG_DEBUG("power_v = %d", power_v);
        LOG_DEBUG("power_str = %s", power_str);
        strcpy(data, power_str);
    }
    json_decref(root);
    return 0;
}

static char* int_to_string(int num)
{
    // Проверка на отрицательное число
    if (num < 0) {
        return "NULL";
    }
    // Специальная обработка нуля
    if (num == 0) {
        char* str = malloc(2);
        if (str) {
            str[0] = '0';
            str[1] = '\0';
        }
        return str;
    }
    // Подсчет количества цифр
    int temp = num;
    int length = 0;
    while (temp > 0) {
        length++;
        temp /= 10;
    }
    // Выделение памяти под строку
    char* str = malloc(length + 1);
    if (!str) {
        return "NULL";
    }
    // Заполнение строки цифрами
    str[length] = '\0';  // Добавляем терминатор
    temp = num;
    for (int i = length - 1; i >= 0; i--) {
        str[i] = (temp % 10) + '0';  // Преобразование цифры в символ
        temp /= 10;
    }
    return str;
}



