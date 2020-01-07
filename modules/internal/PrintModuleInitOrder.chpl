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

//
// This module implements the printModuleInitOrder flag for Chapel
// executables.
//
// If true, it prints the name of the module as it is being
// initialized.  Note that we can't use writeln() here, as the IO
// module has not yet been initialized.
//
pragma "export init"
module PrintModuleInitOrder {
  private use ChapelBase;

  config const printModuleInitOrder = false;
  pragma "print module init indent level" var moduleInitLevel = 2:int(32);

  //
  // Called by all modules during initialization
  //
  pragma "print module init fn"
  proc printModuleInit(s1: c_string, s2: c_string, len: int) {
    extern proc printf(s1: c_string, len: int(32), s2: c_string);
    if printModuleInitOrder then
      printf(s1, moduleInitLevel+len:int(32)+2:int(32), s2);
  }

  proc initPrint() {
    extern proc printf(s: c_string);
    printf ("Initializing Modules:\n");
  }

  if printModuleInitOrder then initPrint();
}
