{
#include <stdio.h>
}
A: ID moreIDs* ';' ;
moreIDs: ',' ID { printf( "moreIDs\n" ); } ;
ID: "[a-z]+" { printf( "ID\n" ); } ;
