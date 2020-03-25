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

module NetworkAtomicTypes {
  use NetworkAtomics;

  private proc isSupported(type T) param {
    return T == bool     ||
           T ==  int(32) || T ==  int(64) ||
           T == uint(32) || T == uint(64) ||
           T == real(32) || T == real(64);
  }

  proc chpl__networkAtomicType(type T) type {
    if T == bool           then return RAtomicBool;
    else if isSupported(T) then return RAtomicT(T);
    else                        return chpl__processorAtomicType(T);
  }
}
