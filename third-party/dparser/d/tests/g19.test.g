{
#include <stdio.h>
}
${declare all_matches A}
A: ID moreIDs* ';' ;
moreIDs: ',' ID { printf( "moreIDs\n" ); } ;
ID: "[a-z]+" { printf( "ID\n" ); } ;
