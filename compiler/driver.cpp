/*
  Copyright 2003-4 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#define EXTERN
#include "geysa.h"
#include "parse.h"
#include "arg.h"
#include "codegen.h"
#include "createAST.h"
#include "files.h"
#include "misc.h"
#include "mysystem.h"
#include "stringutil.h"
#include "cg.h"
#include "dump.h"
#include "analysis.h"
#include "clone.h"
#include "dom.h"
#include "driver.h"
#include "graph.h"
#include "inline.h"
#include "grammar.h"
#include "version.h"
#include "pdb.h"
#include "fun.h"
#include "fa.h"

static void help(ArgumentState *arg_state, char *arg_unused);
static void copyright(ArgumentState *arg_state, char *arg_unused);

static int fdump_html = 0;
static char prelude_filename[FILENAME_MAX] = "prelude";
static char passlist_filename[FILENAME_MAX] = "";
static char log_flags[512] = "";
extern int d_verbose_level;
extern int d_debug_level;
static int suppress_codegen = 0;
static int parser_verbose_non_prelude = 0;
static int rungdb = 0;
static int analyzeNewAST = 0;

int fdce_if1 = 1;
int finline = 0;
int fsimple_inline = 0;
int fgraph = 0;
int fgraph_constants = 0;
int fgraph_frequencies = 0;
int fgraph_vcg = 0;
int fcg = 0;
char system_dir[FILENAME_MAX] = DEFAULT_SYSTEM_DIR;
int print_call_depth = 2;

static ArgumentDescription arg_desc[] = {
 {"prelude", 'p', "Prelude Filename", "P", prelude_filename, "CHPL_PRELUDE", NULL},
 {"passlist", ' ', "Passlist Filename", "P", passlist_filename, "CHPL_PASSLIST", NULL},
 {"sysdir", 'S', "System Directory", "P", system_dir, "CHPL_SYSTEM_DIR", NULL},
 {"dce_if1", ' ', "Dead Code Elimination on IF1", "T", &fdce_if1, "CHPL_DCE_IF1", NULL},
 {"print_call_depth", 'C', "Print Call Depth", "I", &print_call_depth, "CHPL_PRINT_CALL_DEPTH", NULL},
 {"constants_per_var", ' ', "# of Constants per Var", "I", &num_constants_per_variable, 
  "CHPL_CONSTANTS_PER_VARIABLE", NULL},
 {"inline", ' ', "Inlining", "T", &finline, "CHPL_INLINE", NULL},
 {"simple_inline", ' ', "Simple Inlining", "T", &fsimple_inline, "CHPL_SIMPLE_INLINE", NULL},
 {"html", 't', "Dump Program in HTML", "T", &fdump_html, "CHPL_HTML", NULL},
 {"lowlevel_cg", 'g', "Low Level Code Generation", "T", &fcg, "CHPL_CG", NULL},
 {"graph", 'G', "Dump Program Graphs", "T", &fgraph, "CHPL_GRAPH", NULL},
 {"graph_var", ' ', "Limit Graph to Var", "S80", graph_var, "CHPL_GRAPH_VAR", NULL},
 {"graph_fun", ' ', "Limit Graph to Fun", "S80", graph_fun, "CHPL_GRAPH_FUN", NULL},
 {"graph_vcg", ' ', "VCG Output", "T", &fgraph_vcg, "CHPL_GRAPHVCG", NULL},
 {"graph_constants", ' ', "Graph Constants", "T", &fgraph_constants, 
  "CHPL_GRAPH_CONSTANTS", NULL},
 {"graph_frequencies", ' ', "Graph Frequencies", "T", &fgraph_frequencies, 
  "CHPL_GRAPH_FREQUENCIES", NULL},
 {"log_dir", ' ', "Log Directory", "P", log_dir, "CHPL_LOG_DIR", NULL},
 {"log", 'l', "Logging Flags", "S512", log_flags, "CHPL_LOG_FLAGS", log_flags_arg},
 {"output", 'o', "Name of Executable Output", "P", executableFilename, "CHPL_EXE_NAME", NULL},
 {"savec", ' ', "Save Intermediate C Code", "P", saveCDir, "CHPL_SAVEC_DIR", NULL},
 {"gdb", ' ', "Run compiler in gdb", "F", &rungdb, NULL, NULL},
 {"analyzenewast", ' ', "Analyze New AST", "F", &analyzeNewAST, NULL, NULL},
 {"no-codegen", ' ', "Suppress code generation", "F", &suppress_codegen, "CHPL_NO_CODEGEN", NULL},
 {"parser_verbose_np", ' ', "Parser Verbose Non-Prelude", "+", 
  &parser_verbose_non_prelude, "CHPL_PARSER_VERBOSE_NON_PRELUDE", NULL},
 {"parser_verbose", 'V', "Parser Verbose Level", "+", &d_verbose_level, 
   "CHPL_PARSER_VERBOSE", NULL},
 {"parser_debug", 'D', "Parser Debug Level", "+", &d_debug_level, "CHPL_PARSER_DEBUG", NULL},
 {"verbose", 'v', "Verbose Level", "+", &verbose_level, "CHPL_VERBOSE", NULL},
 {"print-commands", ' ', "Print Subprocess Commands", "F", &printSystemCommands, 
  "CHPL_PRINT_COMMANDS", NULL},
 {"debug", 'd', "Debug Level", "+", &debug_level, "CHPL_DEBUG", NULL},
 {"copyright", ' ', "Show Copyright", NULL, NULL, NULL, copyright},
 {"help", 'h', "Help (show this list)", NULL, NULL, NULL, help},
 {0}
};

static ArgumentState arg_state = {
  0, 0,
  "program", 
  "path",
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

FrontEnd langs[] = {
#ifdef LANG_V
  {"v", &parser_tables_v, no_preprocessor_whitespace},
#endif
};

int 
is_test_lang(char *fn) {
  char *ext = strrchr(fn, '.');
  if (!ext)
    return 0;
  ext++;
  for (int i = 0; i < (int)numberof(langs); i++)
    if (!strcmp(langs[i].extension, ext))
      return 1;
  return 0;
}

static void
help(ArgumentState *arg_state, char *arg_unused) {
  char ver[30];
  get_version(ver);
  fprintf(stderr, "%s Version %s ", arg_state->program_name, ver);  
  fprintf(stderr, "Copyright (c) 1994-2004 John Plevyak\n");  
  usage(arg_state, arg_unused);
}

static void
copyright(ArgumentState *arg_state, char *arg_unused) {
  fprintf(stderr,
#include "COPYRIGHT"
	  );
  fprintf(stderr, "\n\n");
  clean_exit(0);
}

static ParseAST *
load_file(char *fn, D_Scope **scope, FrontEnd *fe) {
  int len = 0;
  ParseAST *res = 0;
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

static int
load_one(char *fn) {
  int l;
  char tmpfn[1024];
  Vec<ParseAST *> av;
  ParseAST *a;
  D_Scope *scope = NULL;
  char *ext = strrchr(fn, '.');
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
  if (!(a = load_file(tmpfn, &scope, &langs[l])))
    return -1;
  av.add(a);
  { 
    int save_parser_verbose = d_verbose_level;
    if (parser_verbose_non_prelude) d_verbose_level = parser_verbose_non_prelude;
    if (!(a = load_file(fn, &scope, &langs[l])))
      return -1;	
    if (parser_verbose_non_prelude) d_verbose_level = save_parser_verbose;
  }
  av.add(a);
  if (ast_gen_if1(if1, av) < 0)
    fail("fatal error, '%s'\n", fn);
  free_D_Scope(scope, 1);
  return 0;
}

static void
do_analysis(char *fn) {
  if1_finalize(if1);
  if (logging(LOG_IF1))
    if1_write(log_fp(LOG_IF1), if1);
  Sym *init = if1_get_builtin(if1, "init");
  for (int i = 0; i < if1->allclosures.n; i++) {
    Fun *f = new Fun(if1->allclosures.v[i], if1->allclosures.v[i] == init);
    if (!f)
      fail("fatal error, IF1 invalid");
    pdb->add(f);
  }
  FA *fa = pdb->fa;
  if (fa->analyze(if1->top->fun) < 0)
    goto Lfail;
  if (clone(fa, if1->top->fun) < 0)
    goto Lfail;
  if (logging(LOG_TEST_FA))
    log_test_fa(fa);
  forv_Fun(f, fa->funs)
    build_forward_cfg_dominators(f);
  frequency_estimation(fa);
  if (fgraph)
    graph(fa, fn, !fgraph_vcg ? GraphViz : VCG);
  if (finline)
    inline_calls(fa);
  else if (fsimple_inline)
    simple_inline_calls(fa);
  if (fdump_html)
    dump_html(fa, fn);
  if (fcg) {
    cg_write_c(fa, if1->top->fun,  fn);
    cg_compile(fn);
  }
  return;
 Lfail:
  fail("fatal error, program does not type");
}

static int
compile_one(char *fn) {
  bool test_lang = is_test_lang(fn);
  if (!test_lang) {
    fileToAST(fn, d_debug_level);
    if (analyzeNewAST) {
      if1->callback = new ACallbacks;
      init_ast();
      Vec<Stmt *> stmts;
      stmts.add(internalPreludeStmts);
      stmts.add(preludeStmts);
      stmts.add(programStmts);
      stmts.add(entryPoint);
      AST_to_IF1(stmts);
      do_analysis(fn);
    }
    runPasses(passlist_filename, programStmts);
    if (!suppress_codegen)
      codegen(fn, system_dir, programStmts);
    else {
      programStmts->printList(stdout, "\n");
      printf("\n");
    }
    return 0;
  } else {
    if1->callback = new PCallbacks;
    init_ast();
    if (load_one(fn) < 0)
      return -1;
    do_analysis(fn);
    return 0;
  }
}

static void
init_system() {
  new IF1;
  new PDB(if1);
  char cwd[FILENAME_MAX];
  if (system_dir[0] == '.' && (!system_dir[1] || system_dir[1] == '/')) {
    getcwd(cwd, FILENAME_MAX);
    strcat(cwd, system_dir + 1);
    memcpy(system_dir, cwd, sizeof(system_dir));
  }
  if (system_dir[strlen(system_dir)-1] != '/') 
    strcat(system_dir, "/");
}

static void
compute_program_name_loc(char* orig_argv0, char** name, char** loc) {
  char* argv0 = copystring(orig_argv0);
  char* lastslash = strrchr(argv0, '/');
  if (lastslash == NULL) {
    *name = argv0;
    *loc = ".";   // BLC: this is inaccurate; we should search the path.  
                  // It's no less accurate than what we did previously, though.
  } else {
    *lastslash = '\0';
    *name = lastslash+1;
    *loc = argv0;
  }
  strcpy(system_dir, *loc);
}


void runCompilerInGDB(int argc, char* argv[]) {
  char* gdbCommandFilename = createGDBFile(argc, argv);
  char* command = glomstrings(4, "gdb -q ", argv[0]," -x ", gdbCommandFilename);
  int status = mysystem(command, "running gdb", 0);

  clean_exit(status);
}

int
main(int argc, char *argv[]) {
  startCatchingSignals();
  
  compute_program_name_loc(argv[0], &(arg_state.program_name),
			   &(arg_state.program_loc));
  process_args(&arg_state, argc, argv);
  if (arg_state.nfile_arguments < 1)
    help(&arg_state, NULL);
  if (rungdb)
    runCompilerInGDB(argc, argv);
  if (fdump_html || strcmp(log_flags, "") || fgraph)
    init_logs();
  init_system();
  for (int i = 0; i < arg_state.nfile_arguments; i++) 
    if (compile_one(arg_state.file_argument[i])) break;
  free_args(&arg_state);
  clean_exit(0);
  return 0;
}
