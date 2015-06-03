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

module ChapelDynDispHack {
  //
  // This is an incredibly lame hack.  If the test:
  //
  // test/classes/bradc/arrayInClass/arrayDomInClassRecord-simpler.chpl
  //
  // works with the following line removed then it sounds like we've
  // moved to a better world than the one in which I committed this,
  // and we can remove this check.  See the comments in that test for
  // more details.
  //
  assert (here.id >= 0);
}
