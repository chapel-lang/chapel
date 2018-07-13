/*
 * Copyright 2004-2018 Cray Inc.
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


/*

Utility routines for Python programmers

This module is designed to support routines with the goal of helping
Python programmers transition to Chapel or feel more comfortable in
it.

*/

module Pythonic {
  
  /* Iterates over `iterable`, yielding 2-tuples in which the first
     element is a value from `iterable` and the second is its
     corresponding integer in the range ``start..``.  This iterator
     can be used in either serial ``for`` or parallel ``forall``
     loops.

     :arg iterable: the expression over which to iterate
     :arg start: the value from which to start enumerating (defaults to 0)

     :ytype: 2-tuples of the form (`<type yielded by iterable>`, ``int``)

     Note that this iterator is equivalent to Chapel zippered
     iteration of the form:

       ``for[all] (x, i) in zip(iterable, start..)`` ...

     so can be considered a convenience function.
  */

  iter enumerate(iterable, start=0) {
    for (x, i) in zip(iterable, start..) do
      yield (x,i);
  }

  pragma "no doc"
  iter enumerate(param tag: iterKind, iterable, start=1)
   where tag == iterKind.standalone {
    forall (x, i) in zip(iterable, start..) do
      yield (x,i);
  }
}
