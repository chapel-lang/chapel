#ifndef _CHPL_EXIT_H_
#define _CHPL_EXIT_H_

void gdbShouldBreakHere(void);  // must be exposed to avoid dead-code elim.
void cleanup_for_exit(void);    // must be exposed to avoid dead-code elim.

void _chpl_exit_all(int status);  // must be called by all threads
void _chpl_exit_any(int status);  // may be called by any thread

#endif
