/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

//
// CTypes.chpl
//

/*
Access to C types for interoperability purposes

This module provides access to C types.
*/

module CTypes {
  public use SysCTypes;

  pragma "no doc"
  proc chpl_typeMoveWarning(param name: string, param mod: string,
                            param newmod: string = "CTypes") {
    compilerWarning("type '" + name + "' has moved from '" + mod +
                    "' to '" + newmod + "'; please update your " +
                    "'use'/'import' statements accordingly.", errorDepth=2);
  }

  /* The type corresponding to a C float */
  extern type c_float = real(32);

  /* The type corresponding to a C double */
  extern type c_double = real(64);
}
