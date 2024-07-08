/*
 * Copyright © 2019 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include "hwloc.h"

#include <assert.h>

int main(void)
{
  unsigned static_version = HWLOC_API_VERSION;
  unsigned dynamic_version = hwloc_get_api_version();
  assert(static_version == dynamic_version);
  return 0;
}
