//
// Created by sheverdin on 4/17/25.
//

#include "snmp_utils.h"


void int_to_str(int num, char *buffer)
{
    int is_negative = 0;
    char temp[12]; // Максимум 11 символов для INT_MIN + терминатор
    int temp_index = 0;
    int buffer_index = 0;

    // Обработка нуля
    if (num == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    // Определение знака
    if (num < 0) {
        is_negative = 1;
    } else {
        // Работаем с отрицательными числами для обработки INT_MIN
        num = -num;
    }

    // Извлечение цифр
    while (num != 0) {
        int digit = -(num % 10);
        temp[temp_index++] = digit + '0';
        num /= 10;
    }
    // Добавление знака минус
    if (is_negative) {
        buffer[buffer_index++] = '-';
    }
    // Запись цифр в обратном порядке
    while (temp_index > 0) {
        buffer[buffer_index++] = temp[--temp_index];
    }
    // Завершающий нулевой символ
    buffer[buffer_index] = '\0';
}
