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

#ifndef _chpl_comm_locales_h_
#define _chpl_comm_locales_h_

#include "chpltypes.h"

//
// Returns the default number of locales to use for this comm layer if
// the user does not specify a number.  For most comm layers, this
// should probably print a helpful error and exit rather than
// defaulting to anything.  For comm layer "none" a default of 1
// locale makes sense which is why this routine exists.  If the
// routine returns a value, that value needs to be consistent across
// multiple calls to the routine.
//
int64_t chpl_comm_default_num_locales(void);

//
// This routine allows a comm layer to screen the number of locales to
// be used.  In particular, if a number exceeding some sort of maximum
// was provided, an error should be reported.
//
void chpl_comm_verify_num_locales(int64_t proposedNumLocales);


#endif
