#ifndef QIOPLUGIN_LUSTRE_H_
#define QIOPLUGIN_LUSTRE_H_

#include "sys_basic.h"
#include "qio.h"

#ifdef __cplusplus
extern "C" {
#endif

qioerr sys_lustre_get_stripe(qio_file_t* file, int64_t* size_out);

#ifdef __cplusplus
}
#endif
#endif
