#include "chplrt.h"
#include "chplcomm_locales.h"
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
int _chpl_comm_default_num_locales(void) {
  // This is probably a good default for ARMCI:
  chpl_error("Specify number of locales via -nl <#> or --numLocales=<#>", 0, 0);
  return 0;
}


//
// This routine allows a comm layer to screen the number of locales to
// be used.  In particular, if a number exceeding some sort of maximum
// was provided, an error should be reported.
//
void _chpl_comm_verify_num_locales(int32_t proposedNumLocales) {
  // This can probably remain empty for ARMCI unless there is any sort of
  // maximum that you'd need to check against
}
