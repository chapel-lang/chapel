/* Note: This needs to go in a separate file (as opposed to going into sys.c for
 * instance) since we have to compile with a C compiler if we are not using the
 * newest version of lustre (due to the lustreapi not being compatible with C++).
 */
#define QIOPLUGIN_LUSTRE_C

#ifndef _DARWIN_C_SOURCE
#define _DARWIN_C_SOURCE
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifndef SIMPLE_TEST
#include "chplrt.h"
#endif

#include <lustre/lustreapi.h>
#include "qio_plugin_lustre.h"
#include <sys/ioctl.h>

#define SYS_HAS_LLAPI 1

qioerr sys_lustre_get_stripe(qio_file_t* file, int64_t* size_out) 
{
  struct lov_user_md_v1 *lum;
  size_t lum_size = sizeof(*lum) + LOV_MAX_STRIPE_COUNT * sizeof(struct lov_user_ost_data_v1);
  int rc = 0;
  qioerr err = 0;

  lum = qio_calloc(lum_size, 1);

  lum->lmm_magic = LOV_USER_MAGIC_V1;
  lum->lmm_stripe_count = LOV_MAX_STRIPE_COUNT;

  STARTING_SLOW_SYSCALL;
  if (file->fp) {
    rc = ioctl(fileno(file->fp), LL_IOC_LOV_GETSTRIPE, lum);
    *size_out = lum->lmm_stripe_size;
  } else if (file->fd != -1) {
    rc = ioctl(file->fd, LL_IOC_LOV_GETSTRIPE, lum);
    *size_out = lum->lmm_stripe_size;
  } else {
    *size_out = 0;
    QIO_GET_CONSTANT_ERROR(err, EBADF, "No file descriptor provided for file");
  }

  if (rc < 0)  {
    *size_out = 0;
    err = qio_mkerror_errno();
  }
  DONE_SLOW_SYSCALL;

  qio_free(lum);

  return err;
}
