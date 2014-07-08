//
// Qthreads implementation of Chapel communication interface
//
// Copyright 2011 Sandia Corporation. Under the terms of Contract
// DE-AC04-94AL85000, there is a non-exclusive license for use of this work by
// or on behalf of the U.S. Government. Export of this program may require a
// license from the United States Government
//

#ifndef _comm_qthreads_locales_h_
#define _comm_qthreads_locales_h_

#include "chplrt.h"
#include "arg.h"
#include "chpl-comm-locales.h"
#include "error.h"

int64_t chpl_comm_default_num_locales(void) {
  return 0;
}

void chpl_comm_verify_num_locales(int64_t proposedNumLocales) {
}

#endif
