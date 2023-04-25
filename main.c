#include "log.h"

int main( void )
{
    console.prefix( "wrnlb" );
    console.log( "hello" );
    console.printf( "%s %s", "Hello", "World" );

    console.format( "HH:mm:SS" );
    console.info( "in%s", "fo" );
    console.set_color( LOG_PREFIX, LOG_COLOR_BRIGHT_CYAN );
    console.set_color( LOG_TIME, LOG_COLOR_BRIGHT_MAGENTA );
    console.warn( "warn" );
    console.erro( "erro%d", 5 );

    return 0;
}
