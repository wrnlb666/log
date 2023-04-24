#ifndef __LOG_H__
#define __LOG_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>

#define LOG_PATTERN_SIZE    80
#define LOG_PREFIX_SIZE     80
typedef struct log_t
{
    // member functions
    void    (*redirect)     ( const FILE* fp );
    void    (*print)        ( const char* line );
    void    (*println)      ( const char* line );
    void    (*printf)       ( const char* format, ... );
    void    (*info)         ( const char* format, ... );
    void    (*warn)         ( const char* format, ... );
    void    (*erro)         ( const char* format, ... );
    void    (*format)       ( const char* pattern );
    void    (*set_pattern)  ( const char* pattern );
    void    (*prefix)       ( const char* prefix );

    // fields
    FILE*   fp;
    char    pattern[ LOG_PATTERN_SIZE ];
    char    log_prefix[ LOG_PREFIX_SIZE ];
    size_t  prefix_size;

} log_t;


extern log_t console_log;


#endif  // __LOG_H__