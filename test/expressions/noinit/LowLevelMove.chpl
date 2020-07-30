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

pragma "no doc"
module LowLevelMove {
  proc needsDeinit(type t) param {
    return __primitive("needs auto destroy", t);
  }
  proc explicitDeinit(ref r) {
    if needsDeinit(r.type) {
      chpl__autoDestroy(r);
    }
  }
  proc moveInitialize(ref lhs, pragma "no auto destroy" in rhs) {
    if lhs.type != rhs.type {
      compilerError("Cannot moveInitialize with different types");
    }
    if lhs.type == nothing {
      // then do nothing
    } else {
      __primitive("=", lhs, rhs);
    }
  }
  // returns the value referred to by arg
  // arg should be considered uninitialized after this point
  proc moveToReturn(const ref arg) {
    if arg.type == nothing {
      return none;
    } else {
      pragma "no init"
      pragma "no copy"
      pragma "no auto destroy"
      var moved: arg.type;
      __primitive("=", moved, arg);

      // Should this zero the memory in arg when bounds checking is on?
      return moved;
    }
  }

  // TODO: what about moves across locales? issue #15676
  // TODO:  these from sort
  //inline proc shallowCopy(ref A, dst, src, nElts) {
  //inline proc shallowCopy(ref DstA, dst, ref SrcA, src, nElts) {


}
