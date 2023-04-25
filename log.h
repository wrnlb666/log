#ifndef __LOG_H__
#define __LOG_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// define built_in buffer size
#define LOG_PATTERN_SIZE    80
#define LOG_PREFIX_SIZE     80


// size of ANSI escape sequence color blocks
#define LOG_STRLEN_COLOR    5

// this library uses ANSI escape sequence for colors
// https://en.wikipedia.org/wiki/ANSI_escape_code
// define colors
#define LOG_COLOR_RESET     "\033[00m"
#define LOG_COLOR_BLACK     "\033[30m"
#define LOG_COLOR_RED       "\033[31m"
#define LOG_COLOR_GREEN     "\033[32m"
#define LOG_COLOR_YELLOW    "\033[33m"
#define LOG_COLOR_BLUE      "\033[34m"
#define LOG_COLOR_MAGENTA   "\033[35m"
#define LOG_COLOR_CYAN      "\033[36m"
#define LOG_COLOR_WHITE     "\033[37m"
// bright colors
#define LOG_COLOR_BRIGHT_BLACK      "\033[90m"
#define LOG_COLOR_BRIGHT_RED        "\033[91m"
#define LOG_COLOR_BRIGHT_GREEN      "\033[92m"
#define LOG_COLOR_BRIGHT_YELLOW     "\033[93m"
#define LOG_COLOR_BRIGHT_BLUE       "\033[94m"
#define LOG_COLOR_BRIGHT_MAGENTA    "\033[95m"
#define LOG_COLOR_BRIGHT_CYAN       "\033[96m"
#define LOG_COLOR_BRIGHT_WHITE      "\033[97m"


typedef enum log_target_t
{
    LOG_TIME,
    LOG_TEXT,
    LOG_PREFIX,
    LOG_INFO,
    LOG_WARN,
    LOG_ERRO,
} log_target_t;


typedef struct log_t
{
    void    (*redirect)     ( const FILE* fp );
    void    (*log)          ( const char* line );
    void    (*printf)       ( const char* format, ... );
    void    (*info)         ( const char* format, ... );
    void    (*warn)         ( const char* format, ... );
    void    (*erro)         ( const char* format, ... );
    void    (*format)       ( const char* pattern );
    void    (*set_pattern)  ( const char* pattern );
    void    (*prefix)       ( const char* prefix );
    void    (*set_color)    ( log_target_t target, char* color );
} log_t;


extern log_t console;


#endif  // __LOG_H__