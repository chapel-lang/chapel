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

#include "alist.h"
#include "astutil.h"
#include "expr.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"

#include "codegen.h"

GenRet AList::codegen(const char* separator) {
  GenInfo* info = gGenInfo;
  GenRet ret;
  if( info->cfile ) {
    // only for C...
    for_alist(node, *this) {
      ret.c += node->codegen().c;
      if (node->next != tail) {
        ret.c += separator;
      }
    }
  } else {
    for_alist(node, *this) {
      // for LLVM, code generation will place
      // statements into the function with
      // the IRBuilder.
      node->codegen();
    }
  }
  return ret;
}
/*
LLVMRet AList::genLLVM(GenInfo info) {
  LLVMRet ret = {NULL, NULL, NULL, NULL};
  
  for_alist(node, *this) {
    node->genLLVM(info);
  }
  
  return ret;
}
*/
