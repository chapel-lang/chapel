/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _stdchpl_H_
#define _stdchpl_H_

#include "chplrt.h"

#include <errno.h>
#include <math.h>
#include <float.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include <chpl_md.h>

#include "arg.h"
#include "config.h"
#include "chplcast.h"
#include "chplcgfns.h"
#include "chpl-atomics.h"
#include "chpl-bitops.h"
#include "chpl-comm.h"
#include "chpl-comm-diags.h"
#include "chpldirent.h"
#include "chplexit.h"
#include "chpl-export-wrappers.h"
#include "chpl-external-array.h"
#include "chpl-file-utils.h"
#include <chplfp.h>
#include "chplglob.h"
#include "chplio.h"
#include "chplmath.h"
#include "chpl-init.h"
#include "chpl-linefile-support.h"
#include "chpl-mem.h"
#include "chpl-mem-array.h"
#include "chplmemtrack.h"
#include "chpl-prefetch.h"
#include "chpl-privatization.h"
#include "chpl-string.h"
#include "chplsys.h"
#include "chpl-tasks.h"
#include "chpltimers.h"
#include "chpl-topo.h"
#include "chpltypes.h"
#include "chpl-vector-macros.h"
#include "chpl-visual-debug.h"
#include "error.h"

#include "chpl-comm-compiler-macros.h"
#include "chpl-wide-ptr-fns.h"
#include "chpl-mem-consistency.h"

#endif
