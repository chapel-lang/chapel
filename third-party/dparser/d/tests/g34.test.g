{
#include <stdio.h>
}

${declare longest_match identifier}

P: S*;

S: 'do' identifier
  { printf("do identifier\n"); }
 | identifier
  { printf("identifier\n"); }
 | identifierX
  { printf("identifierX\n"); }
 ;

identifier: "[a-z]+" $term -1;
identifierX: "[a-z]+X" $term -1;
