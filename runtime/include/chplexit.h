#ifndef _CHPL_EXIT_H_
#define _CHPL_EXIT_H_

void gdbShouldBreakHere(void);  // must be exposed to avoid dead-code elim.
void _chpl_exit(int status);

#endif
