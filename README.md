# log
User friendly logging library in pure C.

## Example:
```c
#include "log.h"

int main( void )
{
    console_log.redirect( stdout );

    console_log.prefix( "wrnlb" );
    console_log.info( "in%s\n", "fo" );
    console_log.warn( "xd\n" );
    console_log.erro( "foo\n" );

    return 0;
}
```
