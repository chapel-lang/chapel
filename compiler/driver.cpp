/*
  Copyright 2003-4 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#define EXTERN
#include "geysa.h"

static void help(ArgumentState *arg_state, char *arg_unused);
static void copyright(ArgumentState *arg_state, char *arg_unused);
static char *program_name = 0;

static int fwrite_c = 0;
static int fdump_html = 0;
static char prelude_filename[FILENAME_MAX] = "prelude";
static char log_flags[512];
extern int d_verbose_level;
extern int d_debug_level;

int fdce_if1 = 1;
char system_dir[FILENAME_MAX] = DEFAULT_SYSTEM_DIR;


static ArgumentDescription arg_desc[] = {
 {"prelude", 'p', "Prelude Filename", "P", prelude_filename, "CHPL_PRELUDE", NULL},
 {"sysdir", 'D', "System Directory", "P", system_dir, "CHPL_SYSTEM_DIR", NULL},
 {"dce_if1", ' ', "Dead Code Elimination on IF1", "T", &fdce_if1, "CHPL_DCE_IF1", NULL},
 {"write_c", ' ', "Write C", "T", &fwrite_c, "CHPL_WRITE_C", NULL},
 {"dump_html", 't', "Dump Program in HTML", "T", &fdump_html, "CHPL_DUMP_HTML", NULL},
 {"log_dir", ' ', "Log Directory", "P", log_dir, "CHPL_LOG_DIR", NULL},
 {"log", 'l', "Logging Flags", "S512", log_flags, "CHPL_LOG_FLAGS", log_flags_arg},
 {"parser_verbose", 'V', "Parser Verbose Level", "+", &d_verbose_level, 
   "CHPL_PARSER_VERBOSE", NULL},
 {"parser_debug", 'D', "Parser Debug Level", "+", &d_debug_level, "CHPL_PARSER_DEBUG", NULL},
 {"verbose", 'v', "Verbose Level", "+", &verbose_level, "CHPL_VERBOSE", NULL},
 {"debug", 'd', "Debug Level", "+", &debug_level, "CHPL_DEBUG", NULL},
 {"copyright", ' ', "Show Copyright", NULL, NULL, NULL, copyright},
 {"help", 'h', "Help (show this list)", NULL, NULL, NULL, help},
 {0}
};

static ArgumentState arg_state = {
  0, 0,
  "program", 
  arg_desc
};

struct FrontEnd {
  char *extension;
  D_ParserTables *compilation_tables;
  D_WhiteSpaceFn whitespace;
};

#ifdef LANG_V
extern D_ParserTables parser_tables_v;
extern D_Symbol d_symbols_v[];
#endif

#ifdef LANG_CHPL
extern D_ParserTables parser_tables_chpl;
extern D_Symbol d_symbols_chpl[];
#endif

FrontEnd langs[] = {
#ifdef LANG_V
  {"v", &parser_tables_v, no_preprocessor_whitespace},
#endif
#ifdef LANG_CHPL
  {"chpl", &parser_tables_chpl, no_preprocessor_whitespace},
#endif
};

static void
help(ArgumentState *arg_state, char *arg_unused) {
  char ver[30];
  get_version(ver);
  fprintf(stderr, "%s Version %s ", program_name, ver);  
  fprintf(stderr, "Copyright (c) 1994-2004 John Plevyak\n");  
  usage(arg_state, arg_unused);
}

static void
copyright(ArgumentState *arg_state, char *arg_unused) {
  fprintf(stderr,
#include "COPYRIGHT"
	  );
  fprintf(stderr, "\n\n");
}

static AST *
load_file(char *fn, IF1 *if1, D_Scope **scope, FrontEnd *fe) {
  int len = 0;
  AST *res = 0;
  D_ParseNode *pn = NULL;
  char *buf = NULL;

  D_Parser *p = new_D_Parser(fe->compilation_tables, sizeof(D_ParseNode_User));
  p->loc.pathname = dupstr(fn);
  p->loc.line = 1;
  p->loc.col = p->loc.previous_col = 0;
  p->save_parse_tree = 1;
  p->initial_white_space_fn = fe->whitespace;
  p->initial_globals = (Globals*)MALLOC(sizeof(Globals));
  memset(p->initial_globals, 0, sizeof(Globals));
  p->initial_globals->i = if1;
  if (*scope)
    p->initial_scope = *scope;

  if (buf_read(fn, &buf, &len) > 0)
    pn = dparse(p, buf, len);
  else 
    fail("unable to read file '%s'", fn);
  if (pn) {
    if (*scope)
      (*scope)->owned_by_user = 0;
    *scope = pn->scope;
    (*scope)->owned_by_user = 1;
    free_D_ParseNode(p, pn);
  }
  if (!pn || p->initial_globals->errors || p->syntax_errors) {
    fn = d_dup_pathname_str(p->loc.pathname);
    if (!pn)
      fprintf(stderr, "fatal error, '%s' line %d\n", fn, p->loc.line);
    else
      fprintf(stderr, "fatal error, '%s'\n", fn);
  } else {
    if (logging(LOG_AST))
      ast_print_recursive(log_fp(LOG_AST), pn->user.ast);
    res = pn->user.ast;
  }
  free_D_Parser(p);
  return res;
}

static IF1 *
load_one(char *fn) {
  int l;
  char tmpfn[1024];
  IF1 *if1 = new IF1;
  Vec<AST *> av;
  AST *a;
  D_Scope *scope = NULL;
  char *ext = strchr(fn, '.');
  if (!ext)
    fail("no file extension '%s'", fn);
  for (l = 0; l < (int)numberof(langs); l++)
    if (langs[l].extension && !strcmp(ext + 1, langs[l].extension))
      break;
  if (l >= (int)numberof(langs))
    fail("unknown extension '%s'", fn);

  strcpy(tmpfn, system_dir);
  strcat(tmpfn, prelude_filename);
  strcat(tmpfn, ".");
  strcat(tmpfn, langs[l].extension);
  if (!(a = load_file(tmpfn, if1, &scope, &langs[l])))
    return 0;
  av.add(a);
  if (!(a = load_file(fn, if1, &scope, &langs[l])))
    return 0;
  av.add(a);
  if (ast_gen_if1(if1, av) < 0)
    fail("fatal error, '%s'\n", fn);
  if1_finalize(if1);
  free_D_Scope(scope, 1);
  if (logging(LOG_IF1))
    if1_write(log_fp(LOG_IF1), if1);
  return if1;
}

static void
my_write_c(FA *fa, Fun *main, char *cfn) {
  Vec<Fun *> ftodo, fdone;
  ftodo.add(main);
  for (int fi = 0; fi < ftodo.n; fi++) {
    Fun *f = ftodo.v[fi];
    Vec<AST *> atodo, adone;
    atodo.add(f->ast);
    printf("fun: %s %X\n", f->sym->name ? f->sym->name : "", (int)f);
    for (int ai = 0; ai < atodo.n; ai++) {
      AST *a = atodo.v[ai];
      ast_print(stdout, a);
      Sym *s = type_info(a);
      if (s)
	printf("type: %s %d\n", s->name ? s->name : "", s->id);
      Vec<Fun *> calls;
      call_info(f, a, calls);
      if (calls.n) {
	printf("calls: ");
	forv_Fun(ff, calls) {
	  printf("%d ", ff->sym->id);
	  if (fdone.set_add(ff))
	    ftodo.add(ff);
	}
	printf("\n");
      }
      forv_AST(aa, *a) {
	if (adone.set_add(aa))
	  atodo.add(aa);
      }
    }
  }
}

static int
compile_one(char *fn) {
  IF1 *if1 = load_one(fn);
  if (!if1) return -1;
  PDB *pdb = new PDB(if1);
  Sym *init = if1_get_builtin(if1, "init");

  for (int i = 0; i < if1->allclosures.n; i++) {
    Fun *f = new Fun(pdb, if1->allclosures.v[i], if1->allclosures.v[i] == init);
    if (!f)
      return -1;
    pdb->add(f);
  }
  FA *fa = pdb->analyze(if1->top->fun);
  if (fa) {
    if (pdb->clone(fa, if1->top->fun) < 0)
      goto Lfail;
    if (fwrite_c) {
      char cfn[512];
      strcpy(cfn, fn);
      strcat(cfn, ".c");
      my_write_c(fa, if1->top->fun, cfn);
    }
    if (fdump_html)
      dump_html(fa, if1->top->fun, fn);
  } else
  Lfail:
    fail("fatal error, program does not type");
  return 0;
}

static void
init_system() {
  char cwd[FILENAME_MAX];
  if (system_dir[0] == '.' && (!system_dir[1] || system_dir[1] == '/')) {
    getcwd(cwd, FILENAME_MAX);
    strcat(cwd, system_dir + 1);
    memcpy(system_dir, cwd, sizeof(system_dir));
  }
  if (system_dir[strlen(system_dir)-1] != '/') 
    strcat(system_dir, "/");
}

int
main(int argc, char *argv[]) {
  program_name = argv[0];
  process_args(&arg_state, argv);
  if (arg_state.nfile_arguments < 1)
    help(&arg_state, NULL);
  init_logs();
  init_system();
  for (int i = 0; i < arg_state.nfile_arguments; i++) 
    if (compile_one(arg_state.file_argument[i])) break;
  free_args(&arg_state);
  return 0;
}
