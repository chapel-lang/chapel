{
#include <stdio.h>
}
${tokenize all_matches A}
A: ID ( ',' ID { printf( "moreIDs\n" ); } )* ';' ;
ID: "[a-z]+" { printf( "ID\n" ); } ;
