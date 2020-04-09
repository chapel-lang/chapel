/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

// chplrt.h
//
//################################################################################
//# Inclusion of this file is enforced by a grep command in Makefile.runtime.foot,
//# so it should only contain macro definitions (directly or indirectly) which we
//# wish to impose on the runtime implementation.
//#
//# Inclusion of files containing extern (variable and function) declarations is
//# not accepted usage.
//################################################################################
//

#ifndef _chplrt_H_
#define _chplrt_H_

#include "sys_basic.h"

#ifndef LAUNCHER

#define _noop(x)

#endif // LAUNCHER

#endif
