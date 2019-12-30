/*
 * Copyright 2004-2020 Cray Inc.
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

/* The `FunctionalOperations` module provides operations that act on
   iterators in a functional programming style.
 */
module FunctionalOperations {
  /* Apply `fn` to each element yielded by this iterator and yield the value
     returned by `fn`.  `fn` must take a single argument and return a value.
   */
  iter _iteratorRecord.map(fn) {
    for x in this do yield fn(x);
  }

  /* Apply `fn` to each element yielded by this iterator and yield the values
     it returns `true` for.  `fn` must take a single argument and return a
     boolean value.
   */
  iter _iteratorRecord.filter(fn) {
    for x in this do if fn(x) then yield x;
  }

  /* Apply `fn` to each element yielded by this iterator and ignore the return
     value from `fn` if any.
   */
  proc _iteratorRecord.consume(fn) {
    for x in this do fn(x);
  }
}
