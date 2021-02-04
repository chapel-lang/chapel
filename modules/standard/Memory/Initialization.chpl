/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

module Initialization {

  inline private proc _move(ref dst: ?t, const ref src: t) {
    __primitive("=", dst, src);
  }

  proc needsDeinit(type t) param {
    return __primitive("needs auto destroy", t);
  }

  proc explicitDeinit(ref r) {
    if needsDeinit(r.type) then chpl__autoDestroy(r);
  }

  proc moveInitialize(ref lhs: ?t, pragma "no auto destroy" in rhs: t) {
    if lhs.type != nothing then _move(lhs, rhs);
  }

  proc moveToValue(const ref arg) {
    if arg.type != nothing {
      return none;
    } else {
      pragma "no init", "no copy", "no auto destroy"
      var result: arg.type;
      _move(result, arg);
      return result;
    }
  }

  // TODO: what about moves across locales? issue #15676
  // TODO:  these from sort
  //inline proc shallowCopy(ref A, dst, src, nElts) {
  //inline proc shallowCopy(ref DstA, dst, ref SrcA, src, nElts) {


}
