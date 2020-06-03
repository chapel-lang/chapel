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

proc chpl__staticAutoLocalCheck(accessBase: [], loopDomain) param {
  if chpl__isArrayView(accessBase) then return false;

  return accessBase.domain.type == loopDomain.type &&
         loopDomain.supportsAutoLocalAccess();
}

proc chpl__staticAutoLocalCheck(accessBase, loopDomain) param {
  return false;
}

// these type overloads are for degenerate cases where the optimization can
// break a meaningful error message without these
proc chpl__staticAutoLocalCheck(type accessBase, type loopDomain) param {
  return false;
}
proc chpl__staticAutoLocalCheck(accessBase, type loopDomain) param {
  return false;
}
proc chpl__staticAutoLocalCheck(type accessBase, loopDomain) param {
  return false;
}

proc chpl__dynamicAutoLocalCheck(accessBase, loopDomain) {
  if chpl__staticAutoLocalCheck(accessBase, loopDomain) then
    return accessBase.domain == loopDomain;
  else
    return false;
}

// these type overloads are for degenerate cases where the optimization can
// break a meaningful error message without these
proc chpl__dynamicAutoLocalCheck(type accessBase, type loopDomain) {
  return false;
}
proc chpl__dynamicAutoLocalCheck(accessBase, type loopDomain) {
  return false;
}
proc chpl__dynamicAutoLocalCheck(type accessBase, loopDomain) {
  return false;
}

