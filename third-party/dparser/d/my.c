#include <stdio.h>
#include "dparse.h"
extern D_ParserTables parser_tables_gram;
int
main(int argc, char *argv[]) {
  char s[256], *ss;
  D_Parser *p = new_D_Parser(&parser_tables_gram, sizeof(D_ParseNode_User));
  if (fgets(s,255,stdin) && dparse(p, s, strlen(s)) && !p->syntax_errors)
    printf("success\n");
  else
    printf("failure\n");
}
