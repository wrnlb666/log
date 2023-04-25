#include "log.h"


int main( void )
{
    console.set_color( LOG_PREFIX, LOG_COLOR_BRIGHT_CYAN );
    console.prefix( "wrnlb" );
    console.log( "hello" );
    console.printf( "%s %s", "Hello", "World" );

    console.info( "in%s", "fo" );
    console.prefix( "" );
    console.warn( "xd" );
    console.erro( "foo%d", 5 );

    return 0;
}
