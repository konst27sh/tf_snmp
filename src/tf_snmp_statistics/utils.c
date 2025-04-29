//
// Created by sheverdin on 4/18/25.
//

#include <stdio.h>
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>

json_t* getData_formJson(char* json_data) {
    json_t *root = NULL;
    json_error_t error;
    root = json_loads(json_data, 0, &error);
    if (!root) {
        LOG_ERROR("error: on line %d: %s", error.line, error.text);
    }
    return root;
}

uint16_t int_to_string(int num, char *str)
{
    uint16_t length = 0;
    // Проверка на отрицательное число
    if (num < 0) {
        return 0xFF;
    }
    // Специальная обработка нуля
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        length = 2;
        return length;
    }
    // Подсчет количества цифр
    int temp = num;
    while (temp > 0) {
        length++;
        temp /= 10;
    }
    // Выделение памяти под строку

    // Заполнение строки цифрами
    str[length] = '\0';  // Добавляем терминатор
    temp = num;
    for (int i = length - 1; i >= 0; i--) {
        str[i] = (temp % 10) + '0';  // Преобразование цифры в символ
        temp /= 10;
    }
    return length;
}



