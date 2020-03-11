/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
module HaltWrappers {

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

  /*
     Halt wrapper for an exhaustive select stmt. For more info see
     https://github.com/chapel-lang/chapel/issues/10192
   */
  pragma "function terminates program"
  pragma "always propagate line file info"
  proc exhaustiveSelectHalt(s:string) {
    halt(s);
  }


  //
  // Halt wrappers for misc runtime time checks (where we expect these checks
  // to halt even in an error-handling world.)
  //

  /* Halt wrapper for zippered iteration of unequal lengths */
  pragma "function terminates program"
  pragma "always propagate line file info"
  proc zipLengthHalt(s:string) {
    halt(s);
  }

  /* Halt wrapper for out of memory that mimics the runtime error */
  pragma "function terminates program"
  pragma "always propagate line file info"
  proc outOfMemoryHalt(s:string) {
    const err = "Out of memory allocating \"" + s + "\"";
    __primitive("chpl_error", err.localize().c_str());
  }


  //
  // Halt wrappers for runtime semantic checks (where we expect these to halt
  // when the checks are enabled even in an error-handling world.)
  //

  /* Halt wrapper for --nil-checks */
  pragma "function terminates program"
  pragma "always propagate line file info"
  proc nilCheckHalt(s:string) {
    halt(s);
  }

  /* Halt wrapper for --bounds-checks */
  pragma "function terminates program"
  pragma "always propagate line file info"
  proc boundsCheckHalt(s:string) {
    halt(s);
  }

  /* Halt wrapper for --formal-domain-checks */
  pragma "function terminates program"
  pragma "always propagate line file info"
  proc formalDomainCheckHalt(s:string) {
    halt(s);
  }

  /* Halt wrapper for --local-checks */
  pragma "function terminates program"
  pragma "always propagate line file info"
  proc localCheckHalt(s:string) {
    halt(s);
  }

  /* Halt wrapper for --cast-checks */
  pragma "function terminates program"
  pragma "always propagate line file info"
  proc safeCastCheckHalt(s:string) {
    halt(s);
  }

  /* Halt wrapper for --div-by-zero */
  pragma "function terminates program"
  pragma "always propagate line file info"
  proc divByZeroCheckHalt(s:string) {
    halt(s);
  }

  /* Halt wrapper for unimplemented features */
  pragma "function terminates program"
  pragma "always propagate line file info"
  proc unimplementedFeatureHalt(subject:string, feature: string) {
    halt(subject, " doesn't support ", feature, " yet\n"+
         "  (please feel encouraged to file a GitHub issue requesting this:\n"+
         "   https://github.com/chapel-lang/chapel/issues)");
  }
}
