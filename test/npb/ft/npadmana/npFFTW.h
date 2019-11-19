/* A helper function to see if an FFTW plan is NULL
   or not.

   We can't do this easily in Chapel, because the planner
   type is opaque.
*/

#include "fftw3.h"

static int isNullPlan(fftw_plan plan) {
  return (plan==NULL);
}
