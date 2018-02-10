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


/* Support for a Barrier between a number of tasks across all Locales. */

module AllLocalesBarriers {
  use BlockDist, Barriers;

  private var allLocalesBarrierCreated: atomic bool;
  record AllLocalesBarrier {
    // TODO use PrivateDist?
    const BarrierSpace = LocaleSpace dmapped Block(LocaleSpace);
    var globalBarrier: [BarrierSpace] Barrier;

    proc init(numTasksPerLocale: int) {
      if allLocalesBarrierCreated.testAndSet() then
        halt("Sorry, only one all locales barrier can be created at this time");
      globalBarrier = [b in BarrierSpace] new Barrier(numTasksPerLocale, hackIntoCommBarrier=true);
    }

    proc barrier() {
      globalBarrier.localAccess[here.id].barrier();
    }
  }
}
