#include <stdio.h>
#include "codegen.h"
#include "files.h"
#include "mysystem.h"
#include "stringutil.h"

// BLC -- TODO: 
//   generalize write to use actual types of arguments

static void genAST(FILE* outfile, AST* ast);

static int ast_constant_print(FILE *fp, AST *ast) {
  Sym *s = ast->sym->type;
  switch (s->num_type) {
    case IF1_NUM_TYPE_NONE:
      break;
    case IF1_NUM_TYPE_UINT: {
      switch (s->num_index) {
      case IF1_INT_TYPE_8:
        return fprintf(fp, "%u", ast->sym->imm.v_uint8);
      case IF1_INT_TYPE_16:
        return fprintf(fp, "%u", ast->sym->imm.v_uint16);
      case IF1_INT_TYPE_32:
        return fprintf(fp, "%u", ast->sym->imm.v_uint32);
      case IF1_INT_TYPE_64:
        return fprintf(fp, "%llu", ast->sym->imm.v_uint64);
      default: assert(!"case");
      }
      break;
    }
    case IF1_NUM_TYPE_INT: {
      switch (s->num_index) {
      case IF1_INT_TYPE_8:
        return fprintf(fp, "%d", ast->sym->imm.v_int8);
      case IF1_INT_TYPE_16:
        return fprintf(fp, "%d", ast->sym->imm.v_int16);
      case IF1_INT_TYPE_32:
        return fprintf(fp, "%d", ast->sym->imm.v_int32);
      case IF1_INT_TYPE_64:
        return fprintf(fp, "%lld", ast->sym->imm.v_int64);
      default: assert(!"case");
      }
      break;
    }
    case IF1_NUM_TYPE_FLOAT:
      switch (s->num_index) {
      case IF1_FLOAT_TYPE_32:
        return fprintf(fp, "%f", ast->sym->imm.v_float32);
      case IF1_FLOAT_TYPE_64: 
        return fprintf(fp, "%f", ast->sym->imm.v_float64);
      default: assert(!"case");
      }
      break;
  }
  return -1;
}


static int handleWrite(FILE* outfile, AST* ast) {
  AST* fnast = ast->v[0];
  int i;

  if (fnast->kind == AST_qualified_ident &&
      strcmp(fnast->sym->name, "write") == 0) {
    for (i=2; i<ast->n; i++) {
      fprintf(outfile, "_write_integer64(stdout, ");
      genAST(outfile, ast->v[i]);
      fprintf(outfile, ")");
      if (i < ast->n-1) {
	fprintf(outfile, ";\n");
      }
    }
    return 1;
  } else {
    return 0;
  }
}


static void genAST(FILE* outfile, AST* ast) {
  switch (ast->kind) {
  case AST_block:
    fprintf(outfile, "{\n");
    forv_AST(subtree, *ast) {
      genAST(outfile, subtree);
      fprintf(outfile, ";\n");
    }
    fprintf(outfile, "}\n");
    break;

  case AST_const:
    ast_constant_print(outfile, ast);
    break;

  case AST_def_ident:
    {
      Sym* sym = ast->sym;
      Sym* dt = type_info(ast, sym);
      
      fprintf(outfile, "%s %s", dt->name, sym->name);
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
      }
    }
    /* FALL THROUGH */

  default:
    fprintf(outfile, "/* cannot yet handle: %s\n", AST_name[ast->kind]);
    ast_print_recursive(stdout, ast, 2);
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


static void make(char* tmpdirname) {
  char* command = glomstrings(3, "cd ", tmpdirname, " && make");

  mysystem(command, "compiling generated source");
}


static void cpbin(char* tmpdirname, char* binName) {
  char* command = glomstrings(5, "cp ", tmpdirname, "/", binName, " .");

  mysystem(command, "copying binary to final directory");
}


void codegen(FA* fa, char* infilename, char* compilerDir) {
  char* tmpdirname = createtmpdir();
  char* binName = "a.out";
  openMakefile(infilename, compilerDir, binName);

  FILE* outfile = openoutfile(infilename);

  genHeader(outfile);
  genFuns(outfile, fa);

  closefile(outfile);
  closeMakefile();

  make(tmpdirname);
  cpbin(tmpdirname, binName);

  deletetmpdir();
}
