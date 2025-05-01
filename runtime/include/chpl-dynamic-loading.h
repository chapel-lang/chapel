/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

#ifndef _CHPL_DYNAMIC_LOADING_H_
#define _CHPL_DYNAMIC_LOADING_H_

#ifdef __cplusplus
extern "C" {
#endif

// Simple hook used by the compiler to get the ftable pointer in module
// code. Has to exist because making an 'extern...' declaration of the
// 'ftable' in module code leads to strange compiler errors when compiling
// under earlier versions of LLVM that require typed pointers (e.g,. LLVM-14).
// It has the type 'void**' because in practice that is how we use it (and
// it can contain procedure pointers of any type, now).
void** chpl_get_ftable(void);

extern int CHPL_RTLD_LAZY;

void* chpl_dlopen(const char* path, int mode);

void* chpl_dlsym(void* handle, const char* symbol);

int chpl_dlclose(void* handle);

const char* chpl_dlerror(void);

#ifdef __cplusplus
}
#endif

#endif
