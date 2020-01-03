/*
 * Copyright 2004-2020 Cray Inc.
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

#ifndef _chpl_comm_launch_h
#define _chpl_comm_launch_h

//
// This is an optional comm layer function for the launcher to call
// right before launching the user program.  It only does something if
// there is a layer-specific chpl-comm-launch.h that #defines this.
//
#ifndef CHPL_COMM_PRELAUNCH
#define CHPL_COMM_PRELAUNCH()
#endif

#endif
