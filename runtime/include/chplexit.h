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

#ifndef _CHPL_EXIT_H_
#define _CHPL_EXIT_H_

#ifndef LAUNCHER

void cleanup_for_exit(void);    // must be exposed to avoid dead-code elim.

void chpl_exit_all(int status);  // must be called by all threads
void chpl_exit_any(int status);  // may be called by any thread

#else // LAUNCHER

#define chpl_exit_any(status) exit(status)

#endif // LAUNCHER

#endif
