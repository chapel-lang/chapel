#ifndef _EFA_TP_H_
#define _EFA_TP_H_

#include <config.h>

#if HAVE_LTTNG
#include "rxr_tp_def.h"
#define rxr_tracing(tp_name, ...)  lttng_ust_tracepoint(EFA_RDM_TP_PROV, tp_name, __VA_ARGS__)
#else
#define rxr_tracing(tp_name, ...)  while (0) {}
#endif

#endif // _EFA_TP_H_