#ifndef ARRAY_WRAPPERS_UNKNOWN_TYPE_BUG_H_
#define ARRAY_WRAPPERS_UNKNOWN_TYPE_BUG_H_

#include "chpl-external-array.h"

typedef struct Foo {
    chpl_external_array a;
    chpl_opaque_array b;
} Foo;

#endif
