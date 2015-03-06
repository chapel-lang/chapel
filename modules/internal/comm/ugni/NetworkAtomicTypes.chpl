/*
 * Copyright 2004-2015 Cray Inc.
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

module NetworkAtomicTypes {

  proc chpl__networkAtomicType(type base_type) type {
    if base_type==bool then return ratomicflag;
    else if base_type==uint(32) then return ratomic_uint32;
    else if base_type==uint(64) then return ratomic_uint64;
    else if base_type==int(32) then return ratomic_int32;
    else if base_type==int(64) then return ratomic_int64;
    else if base_type==real then return ratomic_real64;
    else {
      compilerWarning("Unsupported network atomic type");
      if base_type==uint(8) then return atomic_uint8;
      else if base_type==uint(16) then return atomic_uint16;
      else if base_type==int(8) then return atomic_int8;
      else if base_type==int(16) then return atomic_int16;
      else compilerError("Unsupported atomic type");
    }
  }

}
