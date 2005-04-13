#include "geysa.h"
#include "driver.h"
#include "parse_structs.h"
#include "dparse.h"
#include "if1.h"
#include "grammar.h"
#include "parse_ast.h"
#include "parse.h"

extern int d_verbose_level;

void no_preprocessor_whitespace(D_Parser *p, d_loc_t *loc, void **p_user_globals);

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

static char _wspace[256] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 0, 1, 1, 1, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  1, 0, 0, 0, 0, 0, 0, 0 /* zero padded */
};

#define wspace(_x) (_wspace[(unsigned char)_x])

void
no_preprocessor_whitespace(D_Parser *p, d_loc_t *loc, void **p_user_globals) {
  int rec = 0;
  char *s = loc->s, *scol;

  if (p->loc.s == s) 
    scol = s; 
  else 
    scol = 0;
 Lmore:
  while (wspace(*s)) s++;
  if (*s == '\n') {
    loc->line++; 
    scol = s + 1;
    s++;
    goto Lmore;
  }
  if (s[0] == '/') {
    if (s[1] == '/') {
      while (*s && *s != '\n') { s++; }
      loc->line++;
      s++;
      goto Lmore;
    }
    if (s[1] == '*') {
      s += 2;
    LnestComment:
      rec++;
    LmoreComment:
      while (*s) {
        if (s[0] == '*' && s[1] == '/') {
          s += 2;
          rec--;
          if (!rec)
            goto Lmore;
          goto LmoreComment;
        }
        if (s[0] == '/' && s[1] == '*') {
          s += 2;
          goto LnestComment;
        }
        if (*s == '\n') { 
          loc->line++; 
          scol = s + 1; 
        }
        s++;
      }
    }
  }
  if (scol)
    loc->col = s - scol;
  else
    loc->col = -1;
  loc->s = s;
  return;
}

static ParseAST *
load_file(char *fn, FrontEnd *fe) {
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

  if (buf_read(fn, &buf, &len) > 0)
    pn = dparse(p, buf, len);
  else 
    fail("unable to read file '%s'", fn);
  /*  John -- is this correct?  -BLC
  if (pn)
    free_D_ParseNode(p, pn);
  */
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
  char *ext = strrchr(fn, '.');
  if (!ext)
    fail("no file extension '%s'", fn);
  for (l = 0; l < (int)numberof(langs); l++)
    if (langs[l].extension && !strcmp(ext + 1, langs[l].extension))
      break;
  if (l >= (int)numberof(langs))
    fail("unknown extension '%s'", fn);

  strcpy(tmpfn, system_dir);
  strcat(tmpfn, "vparser/");
  strcat(tmpfn, prelude_filename);
  strcat(tmpfn, ".");
  strcat(tmpfn, langs[l].extension);
  if (!(a = load_file(tmpfn, &langs[l])))
    return -1;
  av.add(a);
  { 
    int save_parser_verbose = d_verbose_level;
    if (parser_verbose_non_prelude) d_verbose_level = parser_verbose_non_prelude;
    if (!(a = load_file(fn, &langs[l])))
      return -1;        
    if (parser_verbose_non_prelude) d_verbose_level = save_parser_verbose;
  }
  av.add(a);
  if (ast_gen_if1(if1, av) < 0)
    fail("fatal error, '%s'\n", fn);
  return 0;
}

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

int
compile_one_test_file(char *fn) {
extern int d_debug_level;
   d_debug_level = debugParserLevel;
  if1->callback = new PCallbacks;
  init_ast();
  if (load_one(fn) < 0)
    return -1;
  do_analysis(fn);
  return 0;
}
