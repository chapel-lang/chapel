{
#include <stdio.h>
}

${declare all_matches S}
${declare longest_match do}

P: S*;

S: do identifier
  { printf("do identifier\n"); }
 | identifier
  { printf("identifier\n"); }
 | identifierX
  { printf("identifierX\n"); }
 ;

do ::= 'do';
identifier: "[a-z]+" $term -1;
identifierX: "[a-z]+X" $term -1;
