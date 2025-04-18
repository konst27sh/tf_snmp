//
// Created by sheverdin on 4/18/25.
//

#ifndef TF_SNMP_MAIN_SNMP_H
#define TF_SNMP_MAIN_SNMP_H

#ifdef DEBUG
#define LOG_LEVEL(level, color, fmt, ...) \
    do { \
        fprintf(stderr, \
            "\033[1;" color "m[%-5s]\033[0m " \
            "\033[2;37m%s:%d %s()\033[0m " \
            "\033[" color "m" fmt "\033[0m\n", \
            level, __FILE__, __LINE__, __func__ __VA_OPT__(,) __VA_ARGS__); \
        fflush(stderr); \
    } while(0)

    #define LOG_DEBUG(fmt, ...) LOG_LEVEL("DEBUG", "34", fmt, ##__VA_ARGS__)  // Синий
    #define LOG_INFO(fmt, ...)  LOG_LEVEL("INFO",  "32", fmt, ##__VA_ARGS__)  // Зеленый
    #define LOG_WARN(fmt, ...)  LOG_LEVEL("WARN",  "33", fmt, ##__VA_ARGS__)  // Желтый
    #define LOG_ERROR(fmt, ...) LOG_LEVEL("ERROR", "31", fmt, ##__VA_ARGS__)  // Красный
    #define LOG_FATAL(fmt, ...) LOG_LEVEL("FATAL", "35", fmt, ##__VA_ARGS__)  // Пурпурный
#else
    #define LOG_DEBUG(fmt, ...) do {} while(0)
    #define LOG_INFO(fmt, ...)  do {} while(0)
    #define LOG_WARN(fmt, ...)  do {} while(0)
    #define LOG_ERROR(fmt, ...) do {} while(0)
    #define LOG_FATAL(fmt, ...) do {} while(0)
#endif

#endif //TF_SNMP_MAIN_SNMP_H
