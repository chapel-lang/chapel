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

/*
  This module provides support for ugni performance statistics.

  .. warning::
    Intended for use by runtime developers and requires building ugni with
    perfstats enabled. For a user-facing mechanism, see :mod:`CommDiagnostics`

    For best performance it is recommended that you dynamically link to enable
    vDSO (for faster timer calls.)

    Set ``UGNI_PERFSTATS=1`` to build ugni with perfstats enabled and to
    default to dynamic linking.
 */
module UgniPerfStats {

  /* Zero performance counters on the current locale */
  inline proc resetStatsHere() {
    if CHPL_COMM == "ugni" {
      extern proc chpl_comm_statsStartHere();
      chpl_comm_statsStartHere();
    } else {
      compilerWarning("UgniPerfStats is only supported for CHPL_COMM=ugni");
    }
  }

  /* Zero performance counters on all locales */
  proc resetStats() {
    for loc in Locales do on loc {
      resetStatsHere();
    }
    resetStatsHere();
  }

  /* Print stats on the current locale. By default it prints only the local
     stats for events initiated on this locale. Using :var:`sumAllLocales=true`
     will print the sum across all locales.
   */
  inline proc printStatsHere(sumAllLocales=false) {
    if CHPL_COMM == "ugni" {
      extern proc chpl_comm_statsReport(b: bool);
      chpl_comm_statsReport(sumAllLocales);
    } else {
      compilerWarning("UgniPerfStats is only supported for CHPL_COMM=ugni");
    }
  }

  /* Serially print stats for all locales in order. Each locale prints only the
     local stats for events it initiated. To get a sum across all locales use
     :proc:`printStatsHere` with :var:`sumAllLocales=true`.
   */
  proc printStats() {
    for loc in Locales do on loc {
      printStatsHere(false);
    }
  }
}
