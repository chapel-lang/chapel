#include "chplrt.h"
#include "chplthreads.h"

int
_chpl_cobegin (int                      nthreads, 
               _chpl_threadfp_t        *fp, 
               _chpl_threadarg_t       *a, 
               _chpl_cobegin_wkspace_t *twrk) {
  int   t, retv;
  void *fn_retv;                         // drop?
  retv = 0;

  // fork pthreads
  for (t=0; t<nthreads; t++) {
    if ((twrk[t].error = pthread_create (&(twrk[t].thread), 
                                         NULL, 
                                         fp[t], 
                                         a[t]))) {
      // break; // do not abort for now?
    }
    retv |= twrk[t].error;
  }

  // wait on those fork'd
  for (t=0; t<nthreads; t++) {
    if (!twrk[t].error) {                // if we fork'd successfully
      twrk[t].error = pthread_join (twrk[t].thread, (void*) &fn_retv);
      retv |= twrk[t].error;
     }
  }
  return retv;
}
