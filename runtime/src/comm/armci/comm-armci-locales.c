#include "arg.h"
#include "chplrt.h"
#include "chpl-comm-locales.h"
#include "error.h"

//
// Returns the default number of locales to use for this comm layer if
// the user does not specify a number.  For most comm layers, this
// should probably print a helpful error and exit rather than
// defaulting to anything.  For comm layer "none" a default of 1
// locale makes sense which is why this routine exists.  If the
// routine returns a value, that value needs to be consistent across
// multiple calls to the routine.
//
int64_t chpl_comm_default_num_locales(void) {
  return chpl_specify_locales_error();
}


//
// This routine allows a comm layer to screen the number of locales to
// be used.  In particular, if a number exceeding some sort of maximum
// was provided, an error should be reported.
//
void chpl_comm_verify_num_locales(int64_t proposedNumLocales) {
  // This can probably remain empty for ARMCI unless there is any sort of
  // maximum that you'd need to check against
}
