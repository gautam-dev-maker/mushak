#ifndef __LOG_H__
#define __LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

extern UART_HandleTypeDef huart2;

// int _write(int file, char *ptr, int len) {
//     HAL_UART_Transmit(&huart1, (uint8_t *) ptr, len, HAL_MAX_DELAY);
//     return len;
// }

#define LOG_COLOR_BLACK "30"
#define LOG_COLOR_RED "31"
#define LOG_COLOR_GREEN "32"
#define LOG_COLOR_BROWN "33"
#define LOG_COLOR_BLUE "34"
#define LOG_COLOR_PURPLE "35"
#define LOG_COLOR_CYAN "36"
#define LOG_COLOR(COLOR) "\033[0;" COLOR "m"
#define LOG_BOLD(COLOR) "\033[1;" COLOR "m"
#define LOG_RESET_COLOR "\033[0m"
#define LOG_COLOR_E LOG_COLOR(LOG_COLOR_RED)
#define LOG_COLOR_W LOG_COLOR(LOG_COLOR_BROWN)
#define LOG_COLOR_I LOG_COLOR(LOG_COLOR_GREEN)
#define LOG_COLOR_D LOG_COLOR(LOG_COLOR_CYAN)
#define LOG_COLOR_V LOG_COLOR(LOG_COLOR_PURPLE)

typedef enum
{
    LOG_NONE,   /*!< No log output */
    LOG_ERROR,  /*!< Critical errors, software module can not recover on its own */
    LOG_WARN,   /*!< Error conditions from which recovery measures have been taken */
    LOG_INFO,   /*!< Information messages which describe normal flow of events */
    LOG_DEBUG,  /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    LOG_VERBOSE /*!< Bigger chunks of debugging information, or frequent messages which can potentially flood the output. */
} stm32_log_level_t;

void set_log_level(stm32_log_level_t log_level);

#define DEBUG 1
// TODO: Add a macro so that all the printf can be removed if debug is disabled
#define LOGE(TAG, fmt, ...) \
            set_log_level(LOG_ERROR); \
            printf("(%ld) %s: %s(%d):" fmt, HAL_GetTick(), TAG, __FUNCTION__, __LINE__,  __VA_ARGS__); \
            printf("\n");

#define LOGW(TAG, fmt, ...) \
            set_log_level(LOG_WARN); \
            printf("(%ld) %s: %s(%d):" fmt, HAL_GetTick(), TAG, __FUNCTION__, __LINE__,  __VA_ARGS__); \
            printf("\n");

#define LOGI(TAG, fmt, ...) \
            set_log_level(LOG_INFO); \
            printf("(%ld) %s: %s(%d):" fmt, HAL_GetTick(), TAG, __FUNCTION__, __LINE__,  __VA_ARGS__); \
            printf("\n");

#define LOGD(TAG, fmt, ...) \
            set_log_level(LOG_DEBUG); \
            printf("(%ld) %s: %s(%d):" fmt, HAL_GetTick(), TAG, __FUNCTION__, __LINE__,  __VA_ARGS__); \
            printf("\n");

#define LOGV(TAG, fmt, ...) \
            set_log_level(LOG_VERBOSE); \
            printf("(%ld) %s: %s(%d):" fmt, HAL_GetTick(), TAG, __FUNCTION__, __LINE__,  __VA_ARGS__); \
            printf("\n");

#define HAL_ERROR_CHECK(a, statement, ret) \
            if (!(a)) { \
                LOGE(TAG, "%s", statement); \
                return ret; \
            }
#ifdef __cplusplus
}
#endif

#endif
