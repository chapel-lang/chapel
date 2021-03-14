/* $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testlegacycxx.cc $
 * Description: Legacy GASNet-1 correctness tests in C++
 * Copyright (c) 2017, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

// Test inclusion of gasnet.h after gasnetex.h (bug 3704)
#include <gasnetex.h>
#include <gasnet.h>

#include "testlegacy.c"
