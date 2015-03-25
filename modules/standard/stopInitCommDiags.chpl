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

pragma "no use ChapelStandard"

/*
  This is a companion to module :mod:`startInitCommDiags` and has no
  interface of its own.  There is no benefit to naming it in a ``use``
  statement.  See the documentation for :mod:`startInitCommDiags` for
  more information.
 */
module stopInitCommDiags {
  use startInitCommDiags;

  if printInitVerboseComm {
    use CommDiagnostics;
    stopVerboseComm();
  }

  if printInitCommCounts {
    use CommDiagnostics;
    stopCommDiagnostics();
    writeln(getCommDiagnostics());
    resetCommDiagnostics();
  }
}
