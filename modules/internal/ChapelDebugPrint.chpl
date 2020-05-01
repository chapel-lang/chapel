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

/* Debug printing for internal modules.

   In order to work around problems with resolution order
   (commonly, stdout not defined in methods in DefaultRectangular),
   this module implements a debug printing facility that can
   be used before IO.chpl is resolved (or initialized).
 */

module ChapelDebugPrint {
  private use ChapelStandard, SysCTypes;

  proc chpl_debug_stringify(args...) : string {
    var str = "";
    for param i in 0..args.size-1 {
      var tmp = args(i);
      if _can_stringify_direct(tmp) {
        // Call stringify from IO.chpl. Note that stringify
        // is also called on halt, and so needs to handle
        // being resolved before IO.chpl is completely resolved.
        str += stringify(tmp);
      } else {
        str += "?";
      }
    }
    return str;
  }

  // this function allows DefaultRectangular to perform
  // debug output without using stdout/writeln. It is a
  // work around for module ordering issues in resolution.
  proc chpl_debug_writeln(args...) {
    extern proc printf(fmt:c_string, f:c_string);
    var str = chpl_debug_stringify((...args));
    printf("%s\n", str.c_str());
  }

  //
  // When this flag is used during compilation, calls to chpl__testPar
  // will output a message to indicate that a portion of the code has been
  // parallelized.
  //
  // (chpl__testParFlag and related code is here to avoid order
  //  of resolution issues.)

  pragma "no doc"
  config param chpl__testParFlag = false;
  pragma "no doc"
  var chpl__testParOn = false;

  pragma "no doc"
  proc chpl__testParStart() {
    chpl__testParOn = true;
  }

  pragma "no doc"
  proc chpl__testParStop() {
    chpl__testParOn = false;
  }

  pragma "no doc"
  proc chpl__testPar(args...) {
    if chpl__testParFlag && chpl__testParOn {
      // This function is written this way because it is called
      // from DefaultRectangular. This way of writing it works
      // around resolution ordering issues (such as stdout not
      // yet defined).
      const file_cs : c_string = __primitive("chpl_lookupFilename",
                                        __primitive("_get_user_file"));
      var file: string;
      try! {
        file = createStringWithNewBuffer(file_cs);
      }
      const line = __primitive("_get_user_line");
      var str = chpl_debug_stringify((...args));
      extern proc printf(fmt:c_string, f:c_string, ln:c_int, s:c_string);
      printf("CHPL TEST PAR (%s:%i): %s\n", file_cs, line:c_int, str.c_str());
    }
  }
  pragma "no doc"
  proc chpl__testParWriteln(args...) {
    if chpl__testParFlag && chpl__testParOn {
      const file_cs : c_string = __primitive("chpl_lookupFilename",
                                        __primitive("_get_user_file"));
      var file: string;
      try! {
        file = createStringWithNewBuffer(file_cs);
      }
      const line = __primitive("_get_user_line");
      writeln("CHPL TEST PAR (", file, ":", line, "): ", (...args));
    }
  }

}

