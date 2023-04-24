#include "log.h"

#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
#define snprintf    stbsp_snprintf
#define vsnprintf   stbsp_vsnprintf

log_t console_log;

#ifndef __GNUC__
bool init = false;
#endif  // __GNUC__

void log_redirect( const FILE* fp );
void log_print( const char* line );
void log_println( const char* line );
void log_printf( const char* format, ...  );
void log_info( const char* format, ... );
void log_warn( const char* format, ... );
void log_erro( const char* format, ... );
void log_compile_pattern( const char* pattern );
void log_set_pattern( const char* pattern );
void log_set_prefix( const char* prefix );

#ifndef __GNUC__
static void log_init()
{
    console_log.prefix_size = 0;
    console_log.fp          = stderr;
    console_log.redirect    = log_redirect;
    console_log.print       = log_print;
    console_log.println     = log_println;
    console_log.info        = log_info;
    console_log.warn        = log_warn;
    console_log.erro        = log_erro;
    console_log.printf      = log_printf;
    console_log.format      = log_compile_pattern;
    console_log.set_pattern = log_set_pattern;
    console_log.prefix      = log_set_prefix;
    strcpy( console_log.pattern, "%Y-%m-%d %H:%M:%S" );
}
#else
__attribute__((constructor))
static void log_init()
{
    console_log.prefix_size = 0;
    console_log.fp          = stderr;
    console_log.redirect    = log_redirect;
    console_log.print       = log_print;
    console_log.println     = log_println;
    console_log.info        = log_info;
    console_log.warn        = log_warn;
    console_log.erro        = log_erro;
    console_log.printf      = log_printf;
    console_log.format      = log_compile_pattern;
    console_log.set_pattern = log_set_pattern;
    console_log.prefix      = log_set_prefix;
    strcpy( console_log.pattern, "%Y-%m-%d %H:%M:%S" );
}
#endif  // __GNUC__

void log_redirect( const FILE* fp )
{
    if ( fp != NULL )
    {
        console_log.fp = (FILE*) fp;
    }
    else
    {
        fprintf( stderr, "[ERRO]: cannot redirect to NULL. \n" );
        abort();
    }
}

void log_print( const char* line )
{
    #ifndef __GNUC__
        if ( init == false )
        {
            log_init();
            init = true;
        }
    #endif  // __GNUC__

    time_t curr_time = time(NULL);
    struct tm* time_info = localtime( &curr_time );
    char buf[ LOG_PATTERN_SIZE ] = { 0 };
    strftime( buf, LOG_PATTERN_SIZE, console_log.pattern, time_info );

    size_t buf_len = strlen(buf);
    size_t line_len = strlen(line);
    size_t cap = buf_len + line_len + 2;

    #ifdef _malloca
        char* buffer = _malloca( cap );
    #else
        char* buffer = malloc( cap );
    #endif  // _malloca

    snprintf( buffer, cap, "%s %s", buf, line );
    fputs( buffer, console_log.fp );

    #ifdef _malloca
        _freea( buffer );
    #else
        free( buffer );
    #endif  // _malloca
}

void log_println( const char* line )
{
    #ifndef __GNUC__
        if ( init == false )
        {
            log_init();
            init = true;
        }
    #endif  // __GNUC__

    time_t curr_time = time(NULL);
    struct tm* time_info = localtime( &curr_time );
    char buf[ LOG_PATTERN_SIZE ] = { 0 };
    strftime( buf, LOG_PATTERN_SIZE, console_log.pattern, time_info );

    size_t buf_len = strlen(buf);
    size_t line_len = strlen(line);
    size_t cap = buf_len + line_len + 3;

    #ifdef _malloca
        char* buffer = _malloca( cap );
    #else
        char* buffer = malloc( cap );
    #endif  // _malloca

    snprintf( buffer, cap, "%s %s\n", buf, line );
    fputs( buffer, console_log.fp );

    #ifdef _malloca
        _freea( buffer );
    #else
        free( buffer );
    #endif  // _malloca
}

void log_printf( const char* format, ...  )
{
    #ifndef __GNUC__
        if ( init == false )
        {
            log_init();
            init = true;
        }
    #endif  // __GNUC__

    time_t curr_time = time(NULL);
    struct tm* time_info = localtime( &curr_time );
    char buf[ LOG_PATTERN_SIZE ] = { 0 };
    strftime( buf, LOG_PATTERN_SIZE, console_log.pattern, time_info );

    size_t buf_len = strlen(buf);
    va_list list;
    va_start( list, format );
    size_t format_len = vsnprintf( NULL, 0, format, list );
    size_t cap = buf_len + format_len + 2;

    #ifdef _malloca
        char* buffer = _malloca( cap );
    #else
        char* buffer = malloc( cap );
    #endif  // _malloca

    snprintf( buffer, buf_len + 2, "%s ", buf );
    vsnprintf( buffer + buf_len + 1, format_len + 1, format, list );
    va_end(list);
    fputs( buffer, console_log.fp );

    #ifdef _malloca
        _freea( buffer );
    #else
        free( buffer );
    #endif  // _malloca
}

void log_info( const char* format, ...  )
{
    #ifndef __GNUC__
        if ( init == false )
        {
            log_init();
            init = true;
        }
    #endif  // __GNUC__

    time_t curr_time = time(NULL);
    struct tm* time_info = localtime( &curr_time );
    char buf[ LOG_PATTERN_SIZE ] = { 0 };
    strftime( buf, LOG_PATTERN_SIZE, console_log.pattern, time_info );

    size_t buf_len = strlen(buf);
    va_list list;
    va_start( list, format );
    size_t format_len = vsnprintf( NULL, 0, format, list );
    size_t cap = buf_len + format_len + console_log.prefix_size + 10;

    #ifdef _malloca
        char* buffer = _malloca( cap );
    #else
        char* buffer = malloc( cap );
    #endif  // _malloca

    snprintf( buffer, buf_len + 1, "%s", buf );
    snprintf( buffer + buf_len, console_log.prefix_size + 10, " %s[INFO]: ", console_log.log_prefix );
    vsnprintf( buffer + buf_len + console_log.prefix_size + 9, format_len + 1, format, list );
    va_end(list);
    fputs( buffer, console_log.fp );

    #ifdef _malloca
        _freea( buffer );
    #else
        free( buffer );
    #endif  // _malloca
}

void log_warn( const char* format, ...  )
{
    #ifndef __GNUC__
        if ( init == false )
        {
            log_init();
            init = true;
        }
    #endif  // __GNUC__

    time_t curr_time = time(NULL);
    struct tm* time_info = localtime( &curr_time );
    char buf[ LOG_PATTERN_SIZE ] = { 0 };
    strftime( buf, LOG_PATTERN_SIZE, console_log.pattern, time_info );

    size_t buf_len = strlen(buf);
    va_list list;
    va_start( list, format );
    size_t format_len = vsnprintf( NULL, 0, format, list );
    size_t cap = buf_len + format_len + console_log.prefix_size + 10;

    #ifdef _malloca
        char* buffer = _malloca( cap );
    #else
        char* buffer = malloc( cap );
    #endif  // _malloca

    snprintf( buffer, buf_len + 1, "%s", buf );
    snprintf( buffer + buf_len, console_log.prefix_size + 10, " %s[WARN]: ", console_log.log_prefix );
    vsnprintf( buffer + buf_len + console_log.prefix_size + 9, format_len + 1, format, list );
    va_end(list);
    fputs( buffer, console_log.fp );

    #ifdef _malloca
        _freea( buffer );
    #else
        free( buffer );
    #endif  // _malloca
}

void log_erro( const char* format, ...  )
{
    #ifndef __GNUC__
        if ( init == false )
        {
            log_init();
            init = true;
        }
    #endif  // __GNUC__

    time_t curr_time = time(NULL);
    struct tm* time_info = localtime( &curr_time );
    char buf[ LOG_PATTERN_SIZE ] = { 0 };
    strftime( buf, LOG_PATTERN_SIZE, console_log.pattern, time_info );

    size_t buf_len = strlen(buf);
    va_list list;
    va_start( list, format );
    size_t format_len = vsnprintf( NULL, 0, format, list );
    size_t cap = buf_len + format_len + console_log.prefix_size + 10;

    #ifdef _malloca
        char* buffer = _malloca( cap );
    #else
        char* buffer = malloc( cap );
    #endif  // _malloca

    snprintf( buffer, buf_len + 1, "%s", buf );
    snprintf( buffer + buf_len, console_log.prefix_size + 10, " %s[ERRO]: ", console_log.log_prefix );
    vsnprintf( buffer + buf_len + console_log.prefix_size + 9, format_len + 1, format, list );
    va_end(list);
    fputs( buffer, console_log.fp );

    #ifdef _malloca
        _freea( buffer );
    #else
        free( buffer );
    #endif  // _malloca
}

// for example: 2000-08-28 05:30:00
// yyyy-MM-dd HH:mm:ss
void log_compile_pattern( const char* pattern )
{
    #ifndef __GNUC__
        if ( init == false )
        {
            log_init();
            init = true;
        }
    #endif  // __GNUC__

    size_t len = strlen( pattern );
    if ( len >= LOG_PATTERN_SIZE )
    {
        fprintf( stderr, "[ERRO]: pattern length too long. \n" );
        abort();
    }
    memset( console_log.pattern, 0, LOG_PATTERN_SIZE );

    for ( size_t i = 0, j = 0; i < len && j < LOG_PATTERN_SIZE - 1; i++ )
    {
        if ( strncmp( pattern + i, "yyyy", 4 ) == 0 || strncmp( pattern + i, "YYYY", 4 ) == 0 )
        {
            console_log.pattern[j++] = '%';
            console_log.pattern[j++] = 'Y';
            i += 3;
        }
        else if ( strncmp( pattern + i, "MM", 2 ) == 0 )
        {
            console_log.pattern[j++] = '%';
            console_log.pattern[j++] = 'm';
            i += 1;
        }
        else if ( strncmp( pattern + i, "dd", 2 ) == 0 || strncmp( pattern + i, "DD", 2 ) == 0 )
        {
            console_log.pattern[j++] = '%';
            console_log.pattern[j++] = 'd';
            i += 1;
        }
        else if ( strncmp( pattern + i, "HH", 2 ) == 0 || strncmp( pattern + i, "hh", 2 ) == 0 )
        {
            console_log.pattern[j++] = '%';
            console_log.pattern[j++] = 'H';
            i += 1;
        }
        else if ( strncmp( pattern + i, "mm", 2 ) == 0 )
        {
            console_log.pattern[j++] = '%';
            console_log.pattern[j++] = 'M';
            i += 1;
        }
        else if ( strncmp( pattern + i, "ss", 2 ) == 0 || strncmp( pattern + i, "SS", 2 ) == 0 )
        {
            console_log.pattern[j++] = '%';
            console_log.pattern[j++] = 'S';
            i += 1;
        }
        else
        {
            console_log.pattern[j++] = pattern[i];
        }
    }
}

// pattern should be in the format that allows `strftime` to parse
// https://man7.org/linux/man-pages/man3/strftime.3.html
void log_set_pattern( const char* pattern )
{
    size_t len = strlen( pattern );
    if ( len >= LOG_PATTERN_SIZE )
    {
        fprintf( stderr, "[ERRO]: pattern length too long. \n" );
        abort();
    }
    strncpy( console_log.pattern, pattern, LOG_PATTERN_SIZE );
}

void log_set_prefix( const char* prefix )
{
    size_t length = strlen( prefix );
    length += 2;
    if ( length >= LOG_PREFIX_SIZE )
    {
        fprintf( stderr, "[ERRO]: prefix length too long. \n" );
        abort();
    }
    console_log.prefix_size = length;
    snprintf( console_log.log_prefix, LOG_PREFIX_SIZE, "[%s]", prefix );
}

