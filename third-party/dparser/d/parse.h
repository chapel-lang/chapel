/*
  Copyright 2002-2003 John Plevyak, All Rights Reserved
*/
#ifndef _parse_H_
#define _parse_H_

#define NO_DPN          ((D_ParseNode*)0x1)
#define DPN_TO_PN(_dpn) \
  ((PNode *)(((char*)dpn)-(int)(&((PNode*)0)->parse_node)))
#define is_epsilon_PNode(_pn) \
((_pn)->parse_node.start_loc.s == (_pn)->parse_node.end)

/* #define TRACK_PNODES	1 */

struct PNode;
struct SNode;
struct ZNode;
struct Parser;

typedef Vec(struct ZNode*) VecZNode;
typedef Vec(VecZNode *) VecVecZNode;
typedef Vec(struct SNode*) VecSNode;
typedef Vec(struct PNode*) VecPNode;

typedef struct PNodeHash {
  struct PNode	**v;
  uint		i;	/* size index (power of 2) */
  uint  	m;	/* max size (highest prime < i ** 2) */
  uint  	n;	/* size */
  struct PNode  *all;
} PNodeHash;

typedef struct SNodeHash {
  struct SNode  **v;
  uint		i;	/* size index (power of 2) */
  uint  	m;	/* max size (highest prime < i ** 2) */
  uint  	n;	/* size */
  struct SNode  *all;
  struct SNode  *last_all;
} SNodeHash;

typedef struct Reduction {
  struct ZNode		*znode;
  struct SNode		*snode;
  struct D_Reduction	*reduction;
  struct SNode		*new_snode;
  int			new_depth;
  struct Reduction	*next;
} Reduction;

typedef struct Shift {
  struct SNode		*snode;
  struct Shift		*next;
} Shift;

typedef struct Parser {
  D_Parser user;
  /* string to parse */
  char *start, *end;
  struct D_ParserTables *t;
  /* statistics */
  int states, pnodes, scans, shifts, reductions, compares, ambiguities;
  /* parser state */
  PNodeHash pnode_hash;
  SNodeHash snode_hash;
  Reduction *reductions_todo;
  Shift *shifts_todo;
  D_Scope *top_scope;
  struct SNode *accept;
  int last_syntax_error_line;
  /* memory management */
  Reduction *free_reductions;
  Shift *free_shifts;
  int live_pnodes;
  struct PNode *free_pnodes;
  struct SNode *free_snodes;
  struct ZNode *free_znodes;
  Vec(D_Reduction *) error_reductions;
  ShiftResult *shift_results;
  int nshift_results;
  D_Shift *code_shifts;
  int ncode_shifts;
  /* comments */
  struct Parser *whitespace_parser;
  /* interface support */
  void *pinterface1;
#ifdef TRACK_PNODES
  struct PNode *xall;
#endif  
} Parser;

/*
  Parse Node - the 'symbol' and the constructed parse subtrees.
*/
typedef struct PNode {
  uint			hash;
  AssocKind		assoc;
  int			priority;
  AssocKind		op_assoc;
  int			op_priority;
  D_Reduction		*reduction;
  D_Shift		*shift;
#ifndef USE_GC
  uint32		refcount;
#endif
  VecPNode		children;
  uint			height;		/* max tree height */
  uint8			evaluated;
  uint8			error_recovery;
  struct PNode		*all_next;
  struct PNode		*bucket_next;
  struct PNode		*ambiguities;
  struct PNode		*latest;	/* latest version of this PNode */
  char			*ws_before;
  char			*ws_after;
  D_Scope               *initial_scope;
  void                  *initial_globals;
  D_ParseNode		parse_node;	/* public fields */
#ifdef TRACK_PNODES
  struct PNode		*xnext;
  struct PNode		*xprev;
#endif
} PNode;

/*
  State Node - the 'state'.
*/
typedef struct SNode {
  D_State	*state;
  D_Scope	*initial_scope;
  void		*initial_globals;
  d_loc_t	loc;
  uint		depth;	     	/* max stack depth (less loops) */
  PNode		*last_pn;
  VecZNode	zns;
#ifndef USE_GC
  uint32	refcount;
#endif
  struct SNode  *bucket_next;
  struct SNode	*all_next;
} SNode;

/*
  (Z)Symbol Node - holds one of the symbols associated with a state.
*/
typedef struct ZNode {
  PNode		*pn;
  VecSNode	sns;
} ZNode;
#define znode_next(_z) (*(ZNode**)&((_z)->pn))

D_ParseNode * ambiguity_count_fn(D_Parser *pp, int n, D_ParseNode **v);

#endif
