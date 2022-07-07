#include "chplrt.h"
#include "chpl-gpu.h"

void chpl_gpu_launch_kernel_flat(int ln, int32_t fn,
                                 const char* fatbinData, const char* name,
                                 int num_threads, int blk_dim, int nargs, ...) {
  /*int grd_dim = (num_threads+blk_dim-1)/blk_dim;*/

  va_list args;
  va_start(args, nargs);
  /*chpl_gpu_launch_kernel_help(ln, fn,*/
                              /*fatbinData, name,*/
                              /*grd_dim, 1, 1,*/
                              /*blk_dim, 1, 1,*/
                              /*nargs, args);*/
  chpl_gpu_launch_kernel_help_with_tripcount(ln, fn,
                              fatbinData, name,
                              nargs, args, num_threads, blk_dim);
  va_end(args);
}
