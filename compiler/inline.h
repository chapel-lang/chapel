/*
  Copyright 2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _inline_H_
#define _inline_H_


#define DEFAULT_INLINE_PNODES			50
#define SIMPLE_INLINE_PNODES_BELOW		10

int inline_call(FA *fa, Fun *f, AST **a, Fun *target = 0);

int frequency_estimation(FA *fa);		// in place of profiling

int simple_inline_calls(FA *fa);
int inline_calls(FA *fa);

#endif
