#include "cg_funs.h"
#include "codegen.h"
#include "datatype.h"
#include "fun.h"

static Fun* currentFunction;


static int funIsInit(Fun* fn) {
  return (strcmp(fn->sym->name, "__init") == 0);
}


int currentFunIsInit(void) {
  return funIsInit(currentFunction);
}


static void genFunHead(FILE* outfile, Fun* fn) {
  const char* fname = fn->sym->name;
  int numargs;
  int i;

  // something with fn->ast->last()
  if (!funIsInit(fn)) {
    fprintf(outfile, "static ");
  }
  fprintf(outfile, "void %s", fname);

  /* formal parameter list */
  fprintf(outfile, "(");
  numargs = fn->args.n;
  if (numargs < 2 ||
      numargs == 2 && dtIsNullTuple(fn->args.v[1]->type)) {
    fprintf(outfile, "void");
  } else {
    for (i=1; i<numargs; i++) {
      if (i > 1) {
	fprintf(outfile, ", ");
      }
      genDT(outfile, fn->args.v[i]->type);
      fprintf(outfile, " ");
      fprintf(outfile, "%s", fn->args.v[i]->sym->name);
    }
  }
  fprintf(outfile,")");
}


static void genFun(FILE* outfile, Fun* fn) {
  AST* root;

  /* gen prototype */
  if (!funIsInit(fn)) {
    genFunHead(intheadfile, fn);
    fprintf(intheadfile, ";\n");
  }

  /* for init function, gen local vars at global file scope */
  if (funIsInit(fn)) {
    fprintf(intheadfile, "\n");
    fprintf(intheadfile, "/* global variables to file */\n");
    forv_AST(subtree, *(fn->ast)) {
      genASTDecls(intheadfile, subtree);
    }
    fprintf(intheadfile, "\n");
  }

  /* gen function itself */
  genFunHead(outfile, fn);
  fprintf(outfile, " ");

  if (fn->ast->kind == AST_def_fun) {
    root = fn->ast->last();
  } else {
    root = fn->ast;
  }
  genAST(outfile, root);
  fprintf(outfile, "\n\n");
}


void genFuns(FILE* outfile, FA* fa) {
  forv_Fun(fn, fa->funs) {
    // const char* name = fn->sym->name ? fn->sym->name : "<unknown>";
    // const char* sname = fn->sym->in ? fn->sym->in->name : "<unknown>";
    // printf("%s::%s\n", sname, name);
    currentFunction = fn;

    if (strcmp(fn->sym->in->name, "user") == 0) {
      genFun(outfile, fn);
    }
  }
}
