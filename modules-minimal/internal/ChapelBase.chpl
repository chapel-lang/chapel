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

// ChapelBase.chpl
//

module ChapelBase {

  pragma "default string value" extern var defaultStringValue: string = noinit;

  //
  // internal reference type
  //
  pragma "ref"
  pragma "no default functions"
  pragma "no object"
  class _ref {
    var _val;
  }

  pragma "compiler generated"
  pragma "init copy fn"
  inline proc chpl__initCopy(a) {
    // Currently, string representations are shared.
    // (See note on proc =(a:string, b:string) above.)
      return a;
  }

  pragma "donor fn"
  pragma "auto copy fn"
  inline proc chpl__autoCopy(x) return chpl__initCopy(x);

  inline proc chpl__autoDestroy(x: ?t) {
    __primitive("call destructor", x);
  }
}
