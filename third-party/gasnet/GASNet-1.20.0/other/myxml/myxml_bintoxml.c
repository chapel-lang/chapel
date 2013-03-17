/*   $Source: /var/local/cvs/gasnet/other/myxml/myxml_bintoxml.c,v $
 *     $Date: 2009/10/22 20:24:53 $
 * $Revision: 1.3 $
 * Description: code to manage xml data
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#define gasneti_malloc(SZ) malloc(SZ)
#define gasneti_calloc(N,SZ) calloc(N,SZ)
#define gasneti_realloc(PTR,SZ) realloc(PTR,SZ)
#define gasneti_free(PTR) free(PTR)
#define gasneti_assert_always(COND) do{if(!(COND)){ fprintf(stderr, "fatalERROR: %s:%d\n", __FILE__, __LINE__); exit(1);}}while(0)
#include "./myxml.h"
#include "./myxml.c"

int main(int argc, char **argv) {
  myxml_node_t *A;
  FILE *fp;
  if(argc!=2) {
    fprintf(stderr, "usage: %s <binfile>\n", argv[0]);
  }
  fp = fopen(argv[1], "r");
  A = myxml_loadTreeBIN(fp);
  fclose(fp);
  myxml_printTreeXML(stdout, A, " ");
  myxml_destroyTree(A);
}
