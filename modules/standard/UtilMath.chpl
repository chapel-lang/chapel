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

// UtilMath: various math-related utilities that could come handy.

// for correctness, both arguments must be strictly positive
proc divceilpos(m: integral, n: integral) {
  if !isIntType(m.type) || !isIntType(n.type) then
    compilerError("divceilpos() accepts only arguments of signed integer types");
  return (m - 1) / n + 1;
}

// for correctness, both arguments must be strictly positive
proc divfloorpos(m: integral, n: integral) {
  if !isIntType(m.type) || !isIntType(n.type) then
    compilerError("divfloorpos() accepts only arguments of signed integer types");
  return m / n;
}
