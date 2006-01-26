#ifndef _defs_H
#define _defs_H
#include "chpl.h"

#ifdef IFA_EXTERN
#define IFA_EXTERN_INIT(_x) = _x
#else
#define IFA_EXTERN_INIT(_x)
#define IFA_EXTERN extern
#endif

#endif
