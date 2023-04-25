#include "log.h"


int main( void )
{
    console.redirect( stdout );

    console.set_color( LOG_PREFIX, LOG_COLOR_BRIGHT_CYAN );
    console.prefix( "wrnlb" );
    console.log( "hello" );
    console.printf( "%s %s", "Hello", "World" );

    console.info( "in%s", "fo" );
    console.warn( "xd" );
    console.erro( "foo%d", 5 );

    return 0;
}
