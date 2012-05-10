#ifndef QT_MACROS_H
#define QT_MACROS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_UNUSED
#define QUNUSED(x) UNUSED_ ## x __attribute__((unused))
#else
#define QUNUSED(x) UNUSED_ ## x
#endif

#endif
