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


/* Support for a scalable global barrier between all locales. */
module AllLocalesBarriers {
  use BlockDist, Barriers;

  pragma "no doc"
  class AllLocalesBarrier: BarrierBaseType {

    const BarrierSpace = LocaleSpace dmapped Block(LocaleSpace);
    var globalBarrier: [BarrierSpace] Barrier;

    proc init(numTasksPerLocale: int) {
      globalBarrier = [b in BarrierSpace] new Barrier(numTasksPerLocale, hackIntoCommBarrier=true);
    }

    proc barrier() {
      globalBarrier.localAccess[here.id].barrier();
    }

    proc reset(numTasksPerLocale: int) {
      [b in globalBarrier] b.reset(numTasksPerLocale);
    }
  }

  var allLocalesBarrier: AllLocalesBarrier;

  allLocalesBarrier = new AllLocalesBarrier(1);

  pragma "no doc"
  proc deinit() {
    delete allLocalesBarrier;
  }
}
