#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include "codegen.h"
#include "files.h"
#include "fun.h"
#include "misc.h"
#include "mysystem.h"
#include "num.h"

static void genAST(FILE* outfile, AST* ast);

#define IO_WRITE   1
#define IO_WRITELN 2

static void genDT(FILE* outfile, Sym* pdt) {
  if (pdt == NULL || pdt->name == NULL) {
    fprintf(outfile, "/* unknown type */");
  } else {
    fprintf(outfile, "_%s", pdt->name);
  }
}


static int handleWrite(FILE* outfile, AST* ast) {
  AST* fnast = ast->v[0];
  int i;
  int writeType = 0;

  if (strcmp(fnast->sym->name, "write") == 0) {
    writeType = IO_WRITE;
  } else if (strcmp(fnast->sym->name, "writeln") == 0) {
    writeType = IO_WRITELN;
  }

  if (fnast->kind == AST_qualified_ident && writeType) {
    for (i=2; i<ast->n; i++) {
      AST* arg = ast->v[i];
      Sym* argdt = type_info(arg);

      fprintf(outfile, "_write");
      genDT(outfile, argdt);
      fprintf(outfile, "(");
      fprintf(outfile, "stdout, ");
      fprintf(outfile, "_default_format");
      genDT(outfile, argdt);
      fprintf(outfile, ", ");
      genAST(outfile, arg);
      fprintf(outfile, ")");
      if (i < ast->n-1 || writeType == IO_WRITELN) {
	fprintf(outfile, ";\n");
      }
    }
    if (writeType == IO_WRITELN) {
      fprintf(outfile, "_write_linefeed(stdout)");
    }
    return 1;
  } else {
    return 0;
  }
}


static void genASTDecls(FILE* outfile, AST* ast) {
  if (ast->kind == AST_def_ident) {
    Sym* sym = ast->sym;
    Sym* dt = type_info(ast, sym);
      
    genDT(outfile, dt);
    fprintf(outfile, " %s;\n", sym->name);
  }
}


static void genAST(FILE* outfile, AST* ast) {
  switch (ast->kind) {
  case AST_block:
    switch (ast->scope_kind) {
    case 0:
      fprintf(outfile, "{\n");
      break;
    case 1:
      fprintf(outfile, "(");
      break;
    default:
      fprintf(outfile, "/* unexpected scope kind (open) */");
      break;
    }
    forv_AST(subtree, *ast) {
      genASTDecls(outfile, subtree);
    }
    forv_AST(subtree, *ast) {
      genAST(outfile, subtree);
      if (ast->scope_kind == 0) {
	fprintf(outfile, ";\n");
      }
    }
    switch (ast->scope_kind) {
    case 0:
      fprintf(outfile, "}\n");
      break;
    case 1:
      fprintf(outfile, ")");
      break;
    default:
      fprintf(outfile, "/* unexpected scope kind (close) */");
      break;
    }
    break;

  case AST_const:
    fprintf(outfile, "%s", ast->string);
    break;

  case AST_def_ident:
    {
      Sym* sym = ast->sym;
      
      fprintf(outfile, "%s", sym->name);
      if (ast->v[1]) {
	fprintf(outfile, " = ");
	genAST(outfile, ast->v[1]);
      }
    }
    break;

  case AST_qualified_ident:
    fprintf(outfile, "%s", ast->sym->name);
    break;

  case AST_op:
    {
      int i;

      if (strcmp(ast->v[1]->sym->name, "(") == 0 &&
	  strcmp(ast->v[1]->string, "#(") == 0) {
        if (!handleWrite(outfile, ast)) {
	  genAST(outfile, ast->v[0]);
	  fprintf(outfile, "(");
	  for (i=2; i<ast->n; i++) {
	    genAST(outfile, ast->v[i]);
	  }
	  fprintf(outfile, ")");
	}
	break;
      } else if (strcmp(ast->v[1]->sym->name, "+") == 0 &&
		 strcmp(ast->v[1]->string, "#+") == 0) {
	genAST(outfile, ast->v[0]);
	fprintf(outfile, "+");
	genAST(outfile, ast->v[2]);
	break;
      } else if (strcmp(ast->v[1]->sym->name, "*") == 0 &&
		 strcmp(ast->v[1]->string, "#*") == 0) {
	genAST(outfile, ast->v[0]);
	fprintf(outfile, "*");
	genAST(outfile, ast->v[2]);
	break;
     }
    }
    /* FALL THROUGH */

  default:
    fprintf(outfile, "/* cannot yet handle: %s\n", AST_name[ast->kind]);
    ast_print_recursive(outfile, ast, 2);
    fprintf(outfile, "*/\n");
  }
}


static void genFun(FILE* outfile, Fun* fn) {
  const char* fname = fn->sym->name;
  AST* root;

  // something with fn->ast->last()
  fprintf(outfile, "void %s() ", fname);
  if (fn->ast->kind == AST_def_fun) {
    root = fn->ast->last();
  } else {
    root = fn->ast;
  }
  genAST(outfile, root);
}


static void genFuns(FILE* outfile, FA* fa) {
  Vec<Fun*> funs;
  
  funs.copy(fa->funs);
  forv_Fun(fn, funs) {
    // const char* name = fn->sym->name ? fn->sym->name : "<unknown>";
    // const char* sname = fn->sym->in ? fn->sym->in->name : "<unknown>";
    // printf("%s::%s\n", sname, name);

    if (strcmp(fn->sym->in->name, "user") == 0) {
      genFun(outfile, fn);
    }
  }
}


static void genHeader(FILE* outfile) {
  fprintf(outfile, "#include \"stdchpl.h\"\n\n");
}


static void handleInterrupt(int sig) {
  fail("received interrupt");
}


void codegen(FA* fa, char* infilename, char* compilerDir) {
  signal(SIGINT, handleInterrupt);
  signal(SIGSEGV, handleInterrupt);
  createTmpDir();
  openMakefile(infilename, compilerDir);

  FILE* outfile = openoutfile(infilename);

  genHeader(outfile);
  genFuns(outfile, fa);

  closefile(outfile);
  closeMakefile();

  makeAndCopyBinary();

  deleteTmpDir();
  signal(SIGINT, SIG_DFL);
}
