%<
typedef struct ParseNode {
  char *options;
} ParseNode;
#define D_ParseNode_User ParseNode

#include <dparse.h>
#if !defined(__FreeBSD__) || (__FreeBSD_version >= 500000)
#include <stdint.h>
#else
#include <inttypes.h>
#endif
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
FILE *fp;
FILE *hfp;

char *
dupstr(char *s, char *e) {
  int l = e-s;
  char *ss = (char*)malloc(l+1);
  memcpy(ss, s, l);
  ss[l] = 0;
  return ss;
}

char *
catstr(char *s, char *ss) {
  int l = strlen(s) + strlen(ss) + 1;
  char *x = (char*)malloc(l+1);
  strcpy(x, s);
  strcat(x, ss);
  return x;
}

#define STR(_n) dupstr(_n.start_loc.s, _n.end)

static int p_index = 0;
%>

${pass declare_data for_all postorder}
${pass define_data for_all postorder}
${pass build_data for_all postorder}

S: P* {
  hfp = fopen("prim_data.h", "w");
  fprintf(hfp, "#ifndef _prim_data_H\n");
  fprintf(hfp, "#define _prim_data_H\n\n");
  fprintf(hfp, "void prim_init(Primitives *p, IF1 *if1);\n\n");
  d_pass(${parser}, &$n, ${pass declare_data});
  fprintf(hfp, "#endif\n");
  fclose(hfp);

  fp = fopen("prim_data.cpp", "w");
  fprintf(fp, "#include \"prim_data_incs.h\"\n\n");
  d_pass(${parser}, &$n, ${pass define_data});
  fprintf(fp, "\nvoid prim_init(Primitives *p, IF1 *if1) {\n");
  fprintf(fp, "  char *n;\n");
  d_pass(${parser}, &$n, ${pass build_data});
  fprintf(fp, "}\n");
  fclose(fp);
};
P: name string nargs pos nres? argtypes rettypes optional ';' 
  declare_data: {
    fprintf(hfp, "extern Prim *%s;\n", STR($n0));
    $g = (void*)(uintptr_t)p_index++;
    fprintf(hfp, "#define P_%s %d\n", STR($n0), (int)(intptr_t)$g);
  }
  define_data: {
    fprintf(fp, "Prim *%s = 0;\n", STR($n0));
  }
  build_data: {
    int nargs = 0;
    char *rets = STR($n4);
    if (*rets == 0) rets = "1";
    fprintf(fp, "  static PrimType %s_arg_types[] = %s;\n", STR($n0), STR($n5));
    fprintf(fp, "  static PrimType %s_ret_types[] = %s;\n", STR($n0),  STR($n6));
    fprintf(fp, "  %s = new Prim(%d, %s, \"%s\", %s, %s, %s, %s_arg_types, %s_ret_types, %s);\n", 
      STR($n0), (int)(intptr_t)$g, STR($n1), STR($n0), STR($n2), STR($n3), rets, STR($n0),
      STR($n0), $7.options ? $7.options : "0");
    fprintf(fp, "  n = if1->strings.put(%s);\n", STR($n1));
    nargs = atoi(STR($n2));
    nargs -= 2;
    if (nargs < 0)
      nargs = 0;
    fprintf(fp, "  p->prims.add(%s);\n", STR($n0));
    fprintf(fp, "  p->prim_map[%d][%s].put(n, %s);\n", nargs, STR($n3), STR($n0));
  }
;

optional
  : optional 'nonfunctional' 
{
  if (!$0.options)
    $$.options = "PRIM_NON_FUNCTIONAL";
  else {
    $$.options = catstr($$.options, "PRIM_NON_FUNCTIONAL");
  }
}
  |
{
  $$.options = 0;
}
  ;

name: "[a-zA-Z_][a-zA-Z_0-9]*";
string: "\"([^\"\\]|\\[^])*\"";
nargs: num;
pos: num;
nres: num;
num: "-?[0-9]*[uUlL]?";
argtypes: balanced;
rettypes: balanced;
balanced: '{' balanced* '}' | name | ',';

