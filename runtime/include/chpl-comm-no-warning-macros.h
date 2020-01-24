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


// This file should be included to disable warnings
// for runtime components that should be declaring or defining chpl_comm_put,
// etc, directly. Right now that is just chpl-comm.h and the specific 
// comm layer implementations.

#undef chpl_comm_put
#undef chpl_comm_get
#undef chpl_comm_put_strd
#undef chpl_comm_get_strd
