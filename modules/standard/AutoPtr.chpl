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

/* The AutoPtr module provides an implementation of the auto_ptr generic record
   type.  This type may be used to encapsulate a class object, so it is
   automatically released.

   The encapsulated object will be released at the end of the scope in which
   the auto_ptr record is declared -- independent of the scope in which the
   class object is allocated and whether the class object is allocated before
   or after the auto_ptr record.

   * If the auto_ptr is a global variable, then the class object (and its
     containing auto_ptr will be reclaimed at program termination.
   * If the auto_ptr is a function-scope variable, then the class object will
     be reclaimed at function exit.  (If the variable is declared in a nested
     scope, it may be reclaimed before the function exits.)
   * If the auto_ptr is a field in a record or class object, the contained
     class object will be reclaimed when the containing object is destroyed.
*/

module AutoPtr {

  /* The generic auto_ptr record.

Usage:

::

  class C { ... }
  var c = new C( ... );
  var apc = new auto_ptr(c);
  // c will now be destroyed when apc goes out of scope.
  */
  record auto_ptr {
    var ptr;
    proc ~auto_ptr() {
      if ptr != nil then delete ptr;
    }
  }
}
