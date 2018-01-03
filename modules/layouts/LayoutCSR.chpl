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

/* Deprecated. Use the module :mod:`LayoutCS` instead. */
module LayoutCSR {
  use LayoutCS;

  pragma "no doc"
  config param debugCSR = false;

  // TODO: It would be nice to support CSR & CSC type-aliases in LayoutCS
  /* CSR type-alias provided for backwards-compatibility*/
  type CSR = CS;

  compilerWarning(
    "Module 'LayoutCSR' has been deprecated in favor of 'LayoutCS'"
  );

  if debugCSR {
    compilerError("'debugCSR' has been deprecated in favor of 'debugCS'");
  }
}
