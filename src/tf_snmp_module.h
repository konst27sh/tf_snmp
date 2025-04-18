//
// Created by sheverdin on 4/18/25.
//

#ifndef TF_SNMP_MAIN_SNMP_H
#define TF_SNMP_MAIN_SNMP_H

#ifdef DEBUG
    // Уровни логирования (числовые значения)
    #define LOG_LEVEL_DEBUG 0
    #define LOG_LEVEL_INFO  1
    #define LOG_LEVEL_WARN  2
    #define LOG_LEVEL_ERROR 3
    #define LOG_LEVEL_FATAL 4

    // Установка уровня по умолчанию
    #ifndef LOG_LEVEL
        #define LOG_LEVEL LOG_LEVEL_DEBUG
    #endif

    // Базовый макрос
    #define LOG_LEVEL(level, color, lvl_num, fmt, ...) \
        do { \
            if (lvl_num >= LOG_LEVEL) { \
                fprintf(stderr, \
                    "\033[1;" color "m[%-5s]\033[0m " \
                    "\033[2;37m%s:%d %s()\033[0m " \
                    "\033[" color "m" fmt "\033[0m\n", \
                    level, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
                fflush(stderr); \
            } \
        } while(0)

    // Макросы с фильтрацией
    #define LOG_DEBUG(fmt, ...) LOG_LEVEL("DEBUG", "34", LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
    #define LOG_INFO(fmt, ...)  LOG_LEVEL("INFO",  "32", LOG_LEVEL_INFO,  fmt, ##__VA_ARGS__)
    #define LOG_WARN(fmt, ...)  LOG_LEVEL("WARN",  "33", LOG_LEVEL_WARN,  fmt, ##__VA_ARGS__)
    #define LOG_ERROR(fmt, ...) LOG_LEVEL("ERROR", "31", LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
    #define LOG_FATAL(fmt, ...) LOG_LEVEL("FATAL", "35", LOG_LEVEL_FATAL, fmt, ##__VA_ARGS__)
#else
    #define LOG_DEBUG(fmt, ...) do {} while(0)
    #define LOG_INFO(fmt, ...)  do {} while(0)
    #define LOG_WARN(fmt, ...)  do {} while(0)
    #define LOG_ERROR(fmt, ...) do {} while(0)
    #define LOG_FATAL(fmt, ...) do {} while(0)
#endif

#endif //TF_SNMP_MAIN_SNMP_H
