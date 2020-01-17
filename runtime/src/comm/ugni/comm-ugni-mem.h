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

//
// Memory management declarations for the ugni comm layer.
//

#ifndef _comm_ugni_mem_h_
#define _comm_ugni_mem_h_

#include <stdint.h>

#include "chplrt.h"

#include "chpl-mem.h"


void chpl_comm_mem_reg_add_request(size_t size);

void chpl_comm_mem_reg_init(void);

void chpl_comm_mem_reg_tell(void** space_p, size_t* size_p);

void *chpl_comm_mem_reg_allocMany(size_t number, size_t size,
                                  chpl_mem_descInt_t description,
                                  int32_t lineno, int32_t filename);

void chpl_comm_mem_reg_exit(void);

#endif // _comm_ugni_mem_h_
