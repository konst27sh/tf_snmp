//
// Created by sheverdin on 4/18/25.
//

#include "utils.h"
#include "debug.h"
#include "string.h"

json_t* getData_formJson(char* json_data) {
    json_t *root = NULL;
    json_error_t error;
    root = json_loads(json_data, 0, &error);
    if (root == NULL )
    {
        LOG_ERROR("error: on line %d: %s", error.line, error.text);
    }
    return root;
}

int get_i2c_Data(const char* i2c_name, char *res)
{
    char command[128];
    snprintf(command, sizeof(command), "ubus call tf_hwsys getParam '{\"name\":\"%s\"}'", i2c_name);
    LOG_DEBUG("command = %s", command);
    FILE* fp = popen(command, "r");
    if (!fp)
        return 2;

    size_t len = fread(res, 1, 256, fp);
    res[len] = '\0';
    pclose(fp);
    return 0;
}

int get_ubus_Data(const char* command, char *res, uint16_t ubusSize)
{
    LOG_DEBUG("command = %s", command);
    FILE* fp = popen(command, "r");
    if (!fp)
        return 2;

    size_t len = fread(res, 1, ubusSize, fp);
    res[len] = '\0';
    pclose(fp);
    return 0;
}

void get_string_data(char *data, char *res, const char *option)
{
    json_t *root = NULL;
    json_t *value = NULL;
    char *res_temp = NULL;

    if (strlen(data) != 0)
    {
        root = getData_formJson(data);
    }
    if (root != NULL)
    {
        value = json_object_get(root,option);
        if (value != NULL)
        {
            if json_is_string(value)
            {
                res_temp = (char *)json_string_value(value);
            }
        }
    }
    if (res_temp != NULL)
    {
        strncpy(res, res_temp, MAX_BUFFER_SIZE-1);
    }
}

void get_json_data(char *data, const char *option,  const char *section, char *res)
{
    json_t *root = NULL;
    json_t *value = NULL;
    char *res_temp = NULL;

    if (strlen(data) != 0)
    {
        root = getData_formJson(data);
    }
    if (root != NULL)
    {
        value = json_object_get(root,option);
    }
    if (value != NULL)
    {
        value = json_object_get(value,section);
    }
    if (value != NULL)
    {
        if json_is_string(value)
        {
            res_temp = (char *)json_string_value(value);
        }
    }

    if (res_temp != NULL)
    {
        strncpy(res, res_temp, MAX_BUFFER_SIZE-1);
    }
}

void get_json_array(char *data, const char *option,  const char *section, char *res)
{
    json_t *root = NULL;
    json_t *value = NULL;
    char *res_temp = NULL;

    if (strlen(data) != 0)
    {
        root = getData_formJson(data);
    }
    if (root != NULL)
    {
        value = json_object_get(root,option);
    }
    if (value != NULL)
    {
        value = json_array_get(value,0);
    }
    if (value != NULL)
    {
        value = json_object_get(value,section);
    }
    if (value != NULL)
    {
        res_temp = (char *) json_string_value(value);
    }
    if (res_temp != NULL)
    {
        strncpy(res, res_temp, MAX_BUFFER_SIZE-1);
    }
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



