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

#ifndef _chpl_string_h_
#define _chpl_string_h_

#include "chpl-string-support.h"

typedef const char* chpl_string;

#ifdef _stdchpl_H_
/*** only needed for generated code ***/
extern chpl_string defaultStringValue;
#endif

struct chpl_chpl____wide_chpl_string_s;

chpl_string chpl_wide_string_copy(struct chpl_chpl____wide_chpl_string_s* x, int32_t lineno, int32_t filename);

#define CHPL_SHORT_STRING_SIZE 8

typedef struct chpl__inPlaceBuffer_t {
  uint8_t data[CHPL_SHORT_STRING_SIZE];
} chpl__inPlaceBuffer;

uint8_t* chpl__getInPlaceBufferData(chpl__inPlaceBuffer* buf);
uint8_t* chpl__getInPlaceBufferDataForWrite(chpl__inPlaceBuffer* buf);

#endif
