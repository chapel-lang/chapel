/**
 * Coverity Scan model
 *
 * Manage false positives by giving coverity some hints.
 *
 * Updates to this file must be manually submitted by an admin to:
 *
 *   https://scan.coverity.com/projects/1222
 *
 */

// When tag is 1 or 2, let coverity know that execution is halting. Those
// tags correspond to INT_FATAL and USR_FATAL respectively.
//
// INT_FATAL, et al, work by calling setupError and then
// handleError. setupError simply sets some static globals, then handleError
// looks at those to decide how to error. For example, when INT_FATAL calls
// handleError it will abort execution because exit_immediately is true.
//
// Here we're fibbing to coverity by telling it that setupError results in
// abort (as opposed to handleError), but the effect should be the same.
void setupError(const char *filename, int lineno, int tag) {
  if (tag == 1 || tag == 2) {
    __coverity_panic__();
  }
}

//==============================
// runtime
//

//
// exit() ends execution
//
void exit(int status) {
  __coverity_panic__();
}
