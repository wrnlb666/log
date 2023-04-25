#include "log.h"

int main( void )
{
    console.set_color( LOG_PREFIX, LOG_COLOR_BRIGHT_CYAN );
    console.set_color( LOG_TIME, LOG_COLOR_BRIGHT_MAGENTA );
    console.prefix( "wrnlb" );
    console.log( "hello" );
    console.printf( "%s %s", "Hello", "World" );

    console.format( "HH:mm:SS" );
    console.info( "in%s", "fo" );
    console.prefix( "" );
    console.warn( "xd" );
    console.erro( "foo%d", 5 );

    return 0;
}
