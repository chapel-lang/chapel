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

/* TODO

*/
module Math {

  /* Returns the natural logarithm of `x` + 1.

     It is an error if `x` is less than or equal to -1.
  */
  pragma "fn synchronization free"
  extern proc log1p(x: real(64)): real(64);

  /* Returns the natural logarithm of `x` + 1.

     It is an error if `x` is less than or equal to -1.
  */
  inline proc log1p(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc log1pf(x: real(32)): real(32);
    return log1pf(x);
  }
  
  /* Returns the log to the base `2**baseLog2` of the given `in` value.
     If `baseLog2` is `1`, then returns the log to the base `2`;
     if `baseLog2` is `2`, then returns the log to the base `4`, etc.
     Any fractional part is discarded.

     :rtype: `int`
  */
  inline proc logBasePow2(val: int(?w), baseLog2) {
    if (val < 1) {
      halt("Can't take the log() of a non-positive integer");
    }
    return _logBasePow2Help(val, baseLog2);
  }

  /* Returns the log to the base `2**baseLog2` of the given `in` value.
     If `baseLog2` is `1`, then returns the log to the base `2`;
     if `baseLog2` is `2`, then returns the log to the base `4`, etc.
     Any fractional part is discarded.

     :rtype: `int`
  */
  inline proc logBasePow2(val: uint(?w), baseLog2) {
    return _logBasePow2Help(val, baseLog2);
  }

  
}
