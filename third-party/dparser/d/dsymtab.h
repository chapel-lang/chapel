/*
  Copyright 2002-2004 John Plevyak, All Rights Reserved
*/
#ifndef _dsymtab_H_
#define _dsymtab_H_

#ifndef D_UserSym
#define D_UserSym unsigned int
#endif

struct D_SymHash;
struct D_Scope;

typedef struct D_Sym {
  char		 *name;
  int		 len;
  unsigned int	 hash;
  struct D_Scope *scope;
  struct D_Sym	 *update_of;
  struct D_Sym	 *next;
  D_UserSym	 user;
} D_Sym;

#define D_SCOPE_INHERIT			0
#define D_SCOPE_RECURSIVE		1
#define D_SCOPE_PARALLEL		2
#define D_SCOPE_SEQUENTIAL		3

typedef struct D_Scope {
  unsigned int		kind:2;
  unsigned int		owned_by_user:1; /* don't automatically delete */
  unsigned int		depth;
  D_Sym		 	*ll;
  struct D_SymHash	*hash;
  D_Sym		 	*updates;
  struct D_Scope *search;       /* scope to start search */
  struct D_Scope *dynamic;      /* dynamic scope (e.g. methods) */
  struct D_Scope *up;		/* enclosing scope */
  struct D_Scope *up_updates;	/* prior scope in speculative parse */
  struct D_Scope *down;		/* enclosed scopes (for FREE) */
  struct D_Scope *down_next;	/* next enclosed scope */
} D_Scope;

D_Scope *new_D_Scope(D_Scope *parent);
D_Scope *enter_D_Scope(D_Scope *current, D_Scope *scope);
D_Scope *commit_D_Scope(D_Scope *scope);
D_Scope *equiv_D_Scope(D_Scope *scope);
D_Scope *global_D_Scope(D_Scope *scope);
D_Scope *scope_D_Scope(D_Scope *current, D_Scope *scope);
void free_D_Scope(D_Scope *st, int force);
D_Sym *new_D_Sym(D_Scope *st, char *name, char *end, int sizeof_D_Sym);
#define NEW_D_SYM(_st, _name, _end) new_D_Sym(_st, _name, _end, sizeof(D_Sym))
void free_D_Sym(D_Sym *sym);
D_Sym *find_D_Sym(D_Scope *st, char *name, char *end);
D_Sym *find_global_D_Sym(D_Scope *st, char *name, char *end);
/* use for first update in a production to update scope */
D_Sym *update_D_Sym(D_Sym *sym, D_Scope **st, int sizeof_D_Sym);
#define UPDATE_D_SYM(_sym, _st) update_D_Sym(_sym, _st, sizeof(D_Sym))
/* use for first subsequent updates in a production */
D_Sym *update_additional_D_Sym(D_Scope *st, D_Sym *sym, int sizeof_D_Sym);
#define UPDATE_ADDITIONAL_D_SYM(_st, _sym) update_additional_D_Sym(_st, _sym, sizeof(D_Sym))
D_Sym *current_D_Sym(D_Scope *st, D_Sym *sym);
D_Sym *find_D_Sym_in_Scope(D_Scope *st, D_Scope *cur, char *name, char *end);
D_Sym *next_D_Sym_in_Scope(D_Scope **st, D_Sym **sym);
void print_scope(D_Scope *st);

#endif
