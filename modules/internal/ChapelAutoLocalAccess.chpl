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
  // here we do a counter-intuitive trick. Few things of note:
  //
  // 1. if static check fails, than all the accesses to `accessBase` use
  //    regular access, regardless of the return value of this
  // 2. if there are multiple dynamic candidates within a loop, their dynamic
  //    checks are &&'ed by the compiler. So, we want this dynamic check to
  //    not fail because of static failures fewer than the actual number of
  //    dynamic checks
  //
  // To make sure that 2 happens, and relying on the fact in 1, we hardwire
  // this check to be true if the static check fails. So the result of this
  // doesn't affect the &&'ed check
  if chpl__staticAutoLocalCheck(accessBase, loopDomain) == false then
    return true;
  else
    return accessBase.domain == loopDomain;
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

