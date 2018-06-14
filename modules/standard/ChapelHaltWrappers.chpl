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
   This module provides halt wrappers to make it easier to find and identify
   particular use-cases.
 */
module ChapelHaltWrappers {
  use ChapelIO; // for halt()

  //
  // Halt wrappers for cases where we want error-handling, but error-handling
  // isn't supported yet
  //

  /*
     Halt wrapper for cases where we want error-handling in initializers. For
     more info see: https://github.com/chapel-lang/chapel/issues/8793
   */
  pragma "no doc"
  pragma "function terminates program"
  pragma "always propagate line file info"
  proc initHalt(s:string) {
    halt(s);
  }

  /*
     Halt wrapper for cases where we want error-handling in iterators. For more
     info see: https://github.com/chapel-lang/chapel/issues/7134
   */
  pragma "no doc"
  pragma "function terminates program"
  pragma "always propagate line file info"
  proc iterHalt(s:string) {
    halt(s);
  }


  //
  // Halt wrappers for misc cases where a language, compiler, or library
  // feature is missing.
  //

  /*
     Halt wrapper for pure virtual methods. For more info see
     https://github.com/chapel-lang/chapel/issues/8566
   */
  pragma "function terminates program"
  pragma "always propagate line file info"
  proc pureVirtualMethodHalt() {
    halt("pure virtual method called");
  }
}
