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

module ExplicitRefCount {

  pragma "no doc"
  class RefCountBase {

    var refcnt: atomic int;

    /*
      Increment the reference count and return the reference count value before
      the increment operation.
     */
    proc incRefCount() {
      refcnt.add(1);
    }

    /*
      Decrement the reference count and the return reference count value before
      the decrement operation. A return value of 1 indicates that there are no
      more references.
     */
    proc decRefCount() {
      return refcnt.fetchSub(1);
    }

    /*
      Return the current reference count.
     */
    proc getRefCount() {
      return refcnt.peek();
    }

  } // class RefCountBase

} // module ExplicitRefCount
