#include "log.h"

#if true
#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
#define sprintf     stbsp_sprintf
#define vsprintf    stbsp_vsprintf
#define snprintf    stbsp_snprintf
#define vsnprintf   stbsp_vsnprintf
#endif  // true

// fields
log_t   console;
FILE*   log_fp;
char    log_pattern[ LOG_PATTERN_SIZE ];
char    log_prefix[ LOG_PREFIX_SIZE ];
size_t  log_prefix_size;

// color
char*   log_time_color;
char*   log_text_color;
char*   log_prefix_color;
char*   log_info_color;
char*   log_warn_color;
char*   log_erro_color;


#ifndef __GNUC__
bool init = false;
#endif  // __GNUC__

void log_redirect( const FILE* fp );
void log_println( const char* line );
void log_printf( const char* format, ...  );
void log_info( const char* format, ... );
void log_warn( const char* format, ... );
void log_erro( const char* format, ... );
void log_compile_pattern( const char* pattern );
void log_set_pattern( const char* pattern );
void log_set_prefix( const char* prefix );
void log_set_color( log_target_t target, char* color );

#ifndef __GNUC__
static void log_init()
{
    log_prefix_size         = 0;
    log_fp                  = stderr;
    log_time_color          = LOG_COLOR_BRIGHT_GREEN;
    log_text_color          = LOG_COLOR_RESET;
    log_prefix_color        = LOG_COLOR_BRIGHT_CYAN;
    log_info_color          = LOG_COLOR_GREEN;
    log_warn_color          = LOG_COLOR_YELLOW;
    log_erro_color          = LOG_COLOR_RED;
    console.redirect        = log_redirect;
    console.log             = log_println;
    console.info            = log_info;
    console.warn            = log_warn;
    console.erro            = log_erro;
    console.printf          = log_printf;
    console.format          = log_compile_pattern;
    console.set_pattern     = log_set_pattern;
    console.prefix          = log_set_prefix;
    console.set_color       = log_set_color;
    strcpy( log_pattern, "%Y-%m-%d %H:%M:%S" );
}
#else
__attribute__((constructor))
static void log_init()
{
    log_prefix_size         = 0;
    log_fp                  = stderr;
    log_time_color          = LOG_COLOR_BRIGHT_GREEN;
    log_text_color          = LOG_COLOR_RESET;
    log_prefix_color        = LOG_COLOR_BRIGHT_CYAN;
    log_info_color          = LOG_COLOR_GREEN;
    log_warn_color          = LOG_COLOR_YELLOW;
    log_erro_color          = LOG_COLOR_RED;
    console.redirect        = log_redirect;
    console.log             = log_println;
    console.info            = log_info;
    console.warn            = log_warn;
    console.erro            = log_erro;
    console.printf          = log_printf;
    console.format          = log_compile_pattern;
    console.set_pattern     = log_set_pattern;
    console.prefix          = log_set_prefix;
    console.set_color       = log_set_color;
    strcpy( log_pattern, "%Y-%m-%d %H:%M:%S" );
}
#endif  // __GNUC__

void log_redirect( const FILE* fp )
{
    if ( fp != NULL )
    {
        log_fp = (FILE*) fp;
    }
    else
    {
        fprintf( stderr, "[ERRO]: cannot redirect to NULL. \n" );
        abort();
    }
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
    strftime( buf, LOG_PATTERN_SIZE, log_pattern, time_info );
    char* buffer;
    size_t cap;

    if ( log_fp != stdout && log_fp != stderr )
    {
        size_t buf_len = strlen(buf);
        size_t line_len = strlen(line);
        cap = buf_len + line_len + 3;

        #ifdef _malloca
            buffer = _malloca( cap );
        #elif defined alloca
            buffer = cap <= 1024 ? alloca( cap ) : malloc( cap );
        #else
            buffer = malloc( cap );
        #endif  // _malloca

        sprintf( buffer, "%s %s\n", buf, line );
        buffer[ cap - 1 ] = 0;
        fputs( buffer, log_fp );
    }
    else
    {
        size_t buf_len = strlen(buf);
        size_t line_len = strlen(line);
        cap = buf_len + line_len + 3 * LOG_STRLEN_COLOR + 3;

        #ifdef _malloca
            buffer = _malloca( cap );
        #elif defined alloca
            buffer = cap <= 1024 ? alloca( cap ) : malloc( cap );
        #else
            buffer = malloc( cap );
        #endif  // _malloca

        sprintf( buffer, "%s%s %s%s%s\n", log_time_color, buf, log_text_color, line, LOG_COLOR_RESET );
        buffer[ cap - 1 ] = 0;
        fputs( buffer, log_fp );
    }

    #ifdef _malloca
        _freea( buffer );
    #elif defined alloca
        if ( cap > 1024 )
        {
            free( buffer );
        }
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
    strftime( buf, LOG_PATTERN_SIZE, log_pattern, time_info );
    char* buffer;
    size_t cap;


    size_t buf_len = strlen(buf);
    va_list list, lst;
    va_start( list, format );
    va_copy( lst, list );
    size_t format_len = vsnprintf( NULL, 0, format, list );
    va_end(list);

    if ( log_fp != stdout && log_fp != stderr )
    {
        cap = buf_len + format_len + 3;

        #ifdef _malloca
            buffer = _malloca( cap );
        #elif defined alloca
            buffer = cap <= 1024 ? alloca( cap ) : malloc( cap );
        #else
            buffer = malloc( cap );
        #endif  // _malloca

        sprintf( buffer, "%s ", buf );
        vsprintf( buffer + buf_len + 1, format, lst );
        va_end(lst);
        buffer[ cap - 2 ] = 10;
        buffer[ cap - 1 ] = 0;
        fputs( buffer, log_fp );
    }
    else
    {
        cap = buf_len + format_len + 3 * LOG_STRLEN_COLOR + 3;

        #ifdef _malloca
            buffer = _malloca( cap );
        #elif defined alloca
            buffer = cap <= 1024 ? alloca( cap ) : malloc( cap );
        #else
            buffer = malloc( cap );
        #endif  // _malloca

        sprintf( buffer, "%s%s%s ", log_time_color, buf, log_text_color );
        vsprintf( buffer + buf_len + 2 * LOG_STRLEN_COLOR + 1, format, lst );
        sprintf( buffer + buf_len + format_len + 2 * LOG_STRLEN_COLOR + 1, "%s\n", LOG_COLOR_RESET );
        va_end(lst);
        buffer[ cap - 2 ] = 10;
        buffer[ cap - 1 ] = 0;
        fputs( buffer, log_fp );
    }


    #ifdef _malloca
        _freea( buffer );
    #elif defined alloca
        if ( cap > 1024 )
        {
            free( buffer );
        }
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
    strftime( buf, LOG_PATTERN_SIZE, log_pattern, time_info );
    char* buffer;
    size_t cap;

    size_t buf_len = strlen(buf);
    va_list list, lst;
    va_start( list, format );
    va_copy( lst, list );
    size_t format_len = vsnprintf( NULL, 0, format, list );
    va_end( list );

    if ( log_fp != stdout && log_fp != stderr )
    {
        cap = buf_len + format_len + log_prefix_size + 11;

        #ifdef _malloca
            buffer = _malloca( cap );
        #elif defined alloca
            buffer = cap <= 1024 ? alloca( cap ) : malloc( cap );
        #else
            buffer = malloc( cap );
        #endif  // _malloca

        sprintf( buffer, "%s %s[INFO]: ", buf, log_prefix );
        vsprintf( buffer + buf_len + log_prefix_size + 9, format, lst );
        va_end(lst);
        buffer[ cap - 2 ] = 10;
        buffer[ cap - 1 ] = 0;
        fputs( buffer, log_fp );
    }
    else
    {
        cap = buf_len + format_len + log_prefix_size + 4 * LOG_STRLEN_COLOR + 11;

        #ifdef _malloca
            buffer = _malloca( cap );
        #elif defined alloca
            buffer = cap <= 1024 ? alloca( cap ) : malloc( cap );
        #else
            buffer = malloc( cap );
        #endif  // _malloca

        sprintf( buffer, "%s%s %s%s%s[INFO]: ", log_time_color, buf, log_prefix_color, log_prefix, log_info_color );
        vsprintf( buffer + buf_len + log_prefix_size + 3 * LOG_STRLEN_COLOR + 9, format, lst );
        sprintf( buffer + buf_len + format_len + log_prefix_size + 3 * LOG_STRLEN_COLOR + 9, "%s\n", LOG_COLOR_RESET );
        va_end(lst);
        buffer[ cap - 2 ] = 10;
        buffer[ cap - 1 ] = 0;
        fputs( buffer, log_fp );
    }


    #ifdef _malloca
        _freea( buffer );
    #elif defined alloca
        if ( cap > 1024 )
        {
            free( buffer );
        }
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
    strftime( buf, LOG_PATTERN_SIZE, log_pattern, time_info );
    char* buffer;
    size_t cap;

    size_t buf_len = strlen(buf);
    va_list list, lst;
    va_start( list, format );
    va_copy( lst, list );
    size_t format_len = vsnprintf( NULL, 0, format, list );
    va_end( list );

    if ( log_fp != stdout && log_fp != stderr )
    {
        cap = buf_len + format_len + log_prefix_size + 11;

        #ifdef _malloca
            buffer = _malloca( cap );
        #elif defined alloca
            buffer = cap <= 1024 ? alloca( cap ) : malloc( cap );
        #else
            buffer = malloc( cap );
        #endif  // _malloca

        sprintf( buffer, "%s %s[WARN]: ", buf, log_prefix );
        vsprintf( buffer + buf_len + log_prefix_size + 9, format, lst );
        va_end(lst);
        buffer[ cap - 2 ] = 10;
        buffer[ cap - 1 ] = 0;
        fputs( buffer, log_fp );
    }
    else
    {
        cap = buf_len + format_len + log_prefix_size + 4 * LOG_STRLEN_COLOR + 11;

        #ifdef _malloca
            buffer = _malloca( cap );
        #elif defined alloca
            buffer = cap <= 1024 ? alloca( cap ) : malloc( cap );
        #else
            buffer = malloc( cap );
        #endif  // _malloca

        sprintf( buffer, "%s%s %s%s%s[WARN]: ", log_time_color, buf, log_prefix_color, log_prefix, log_warn_color );
        vsprintf( buffer + buf_len + log_prefix_size + 3 * LOG_STRLEN_COLOR + 9, format, lst );
        sprintf( buffer + buf_len + format_len + log_prefix_size + 3 * LOG_STRLEN_COLOR + 9, "%s\n", LOG_COLOR_RESET );
        va_end(lst);
        buffer[ cap - 2 ] = 10;
        buffer[ cap - 1 ] = 0;
        fputs( buffer, log_fp );
    }


    #ifdef _malloca
        _freea( buffer );
    #elif defined alloca
        if ( cap > 1024 )
        {
            free( buffer );
        }
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
    strftime( buf, LOG_PATTERN_SIZE, log_pattern, time_info );
    char* buffer;
    size_t cap;

    size_t buf_len = strlen(buf);
    va_list list, lst;
    va_start( list, format );
    va_copy( lst, list );
    size_t format_len = vsnprintf( NULL, 0, format, list );
    va_end( list );

    if ( log_fp != stdout && log_fp != stderr )
    {
        cap = buf_len + format_len + log_prefix_size + 11;

        #ifdef _malloca
            buffer = _malloca( cap );
        #elif defined alloca
            buffer = cap <= 1024 ? alloca( cap ) : malloc( cap );
        #else
            buffer = malloc( cap );
        #endif  // _malloca

        sprintf( buffer, "%s %s[ERRO]: ", buf, log_prefix );
        vsprintf( buffer + buf_len + log_prefix_size + 9, format, lst );
        va_end(lst);
        buffer[ cap - 2 ] = 10;
        buffer[ cap - 1 ] = 0;
        fputs( buffer, log_fp );
    }
    else
    {
        cap = buf_len + format_len + log_prefix_size + 4 * LOG_STRLEN_COLOR + 11;

        #ifdef _malloca
            buffer = _malloca( cap );
        #elif defined alloca
            buffer = cap <= 1024 ? alloca( cap ) : malloc( cap );
        #else
            buffer = malloc( cap );
        #endif  // _malloca

        sprintf( buffer, "%s%s %s%s%s[ERRO]: ", log_time_color, buf, log_prefix_color, log_prefix, log_erro_color );
        vsprintf( buffer + buf_len + log_prefix_size + 3 * LOG_STRLEN_COLOR + 9, format, lst );
        sprintf( buffer + buf_len + format_len + log_prefix_size + 3 * LOG_STRLEN_COLOR + 9, "%s\n", LOG_COLOR_RESET );
        va_end(lst);
        buffer[ cap - 2 ] = 10;
        buffer[ cap - 1 ] = 0;
        fputs( buffer, log_fp );
    }


    #ifdef _malloca
        _freea( buffer );
    #elif defined alloca
        if ( cap > 1024 )
        {
            free( buffer );
        }
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
    memset( log_pattern, 0, LOG_PATTERN_SIZE );

    for ( size_t i = 0, j = 0; i < len && j < LOG_PATTERN_SIZE - 1; i++ )
    {
        if ( strncmp( pattern + i, "yyyy", 4 ) == 0 || strncmp( pattern + i, "YYYY", 4 ) == 0 )
        {
            log_pattern[j++] = '%';
            log_pattern[j++] = 'Y';
            i += 3;
        }
        else if ( strncmp( pattern + i, "MM", 2 ) == 0 )
        {
            log_pattern[j++] = '%';
            log_pattern[j++] = 'm';
            i += 1;
        }
        else if ( strncmp( pattern + i, "dd", 2 ) == 0 || strncmp( pattern + i, "DD", 2 ) == 0 )
        {
            log_pattern[j++] = '%';
            log_pattern[j++] = 'd';
            i += 1;
        }
        else if ( strncmp( pattern + i, "HH", 2 ) == 0 || strncmp( pattern + i, "hh", 2 ) == 0 )
        {
            log_pattern[j++] = '%';
            log_pattern[j++] = 'H';
            i += 1;
        }
        else if ( strncmp( pattern + i, "mm", 2 ) == 0 )
        {
            log_pattern[j++] = '%';
            log_pattern[j++] = 'M';
            i += 1;
        }
        else if ( strncmp( pattern + i, "ss", 2 ) == 0 || strncmp( pattern + i, "SS", 2 ) == 0 )
        {
            log_pattern[j++] = '%';
            log_pattern[j++] = 'S';
            i += 1;
        }
        else
        {
            log_pattern[j++] = pattern[i];
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
    strncpy( log_pattern, pattern, LOG_PATTERN_SIZE );
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
    log_prefix_size = length;
    snprintf( log_prefix, LOG_PREFIX_SIZE, "[%s]", prefix );
}

void log_set_color( log_target_t target, char* color )
{
    if ( strlen(color) != LOG_STRLEN_COLOR )
    {
        fprintf( stderr, "[ERRO]: invalid color. \n" );
        abort();
    }
    switch ( target )
    {
        case LOG_TIME:
        {
            log_time_color = color;
            return;
        }
        case LOG_TEXT:
        {
            log_text_color = color;
            return;
        }
        case LOG_PREFIX:
        {
            log_prefix_color = color;
            return;
        }
        case LOG_INFO:
        {
            log_info_color = color;
            return;
        }
        case LOG_WARN:
        {
            log_warn_color = color;
            return;
        }
        case LOG_ERRO:
        {
            log_erro_color = color;
            return;
        }
        default:
        {
            fprintf( stderr, "[ERRO]: invalid target. \n" );
            abort();
        }
    }
}
