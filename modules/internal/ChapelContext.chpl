/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

// ChapelContext.chpl
//
// library support code for context managers feature
module ChapelContext {
  import Errors.Error;
  use OwnedObject;

  interface contextManager {
      type contextReturnType;

      pragma "ifc any return intent"
      proc Self.enterContext() ref : contextReturnType;
      proc Self.exitContext(in error: owned Error?);
  }

  proc chpl__verifyTypeContext(x) {
    if __primitive("implements interface", x, contextManager) == 2 then
      compilerWarning(x.type:string + " is being used in a 'manage' statement ",
                      "via its 'enterContext' and 'exitContext' methods. However, " + x.type:string +
                      " does not implement contextManager. ",
                      "In the future, this will result in an error.");
  }
}
