/*
  Copyright 2002-2003 John Plevyak, All Rights Reserved
*/

#ifndef _lex_H_
#define _lex_H_

/* #define LIVE_DIFF_IN_TRANSITIONS */

struct Grammar;

typedef struct ScanStateTransition {
  uint			index;
  VecAction		live_diff;
  VecAction		accepts_diff;
} ScanStateTransition;

typedef struct ScanState {
  uint			index;
  struct ScanState 	*chars[256];
  VecAction		accepts;
  VecAction		live;
  ScanStateTransition	*transition[256];
} ScanState;

void build_scanners(struct Grammar *g);

#endif
