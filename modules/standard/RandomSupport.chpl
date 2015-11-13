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

/*
   Seed generation for pseudorandom number generation

*/
module RandomSupport {

/* 
   An instance of :record:`SeedGenerators` that provides a
   convenient means of generating seeds when the user does not wish to
   specify one manually.
*/
const SeedGenerator: SeedGenerators;


/*
  Provides methods to help generate seeds when the user doesn't want
  to create one.  It currently only supports one such method, but the
  intention is to add more over time.

  .. note::
          Once Chapel supports static class methods,
          :const:`SeedGenerator` and :record:`SeedGenerators` should
          be combined into a single record type with static methods).

*/

record SeedGenerators {
  /*
    Generate a seed based on the current time in microseconds as
    reported by :proc:`Time.getCurrentTime`, ensuring that it
    meets the PRNG's requirements.
  */
  proc currentTime: int(64) {
    use Time;
    const seed: int(64) = getCurrentTime(unit=TimeUnits.microseconds):int(64);
    return seed;

  }
}


} // close module RandomSupport
