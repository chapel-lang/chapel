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

/* Help Module
   
   If the user declares main to take an argument, then Chapel's normal
   processing of --help and -h arguments is suppressed.  As such,
   printUsage allows the user to generate Chapel's normal help
   messages, allowing the user to add information on any additional supported
   arguments.
 */
module Help {
  proc printUsage() {
    extern proc printHelpTable();
    extern proc printConfigVarTable();

    printHelpTable();
    printConfigVarTable();
  }
}
