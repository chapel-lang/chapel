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
  This module defines config params that allow observing inter-locale
  communication during module initialization and teardown.

  It is hard for a programmer to determine exactly what happens during
  initialization or teardown of a module, because the code that runs
  then does so implicitly, as a result of the declarations present.
  And even if that code can be identified, doing debug output or
  logging data for later reporting might not work because the Chapel
  capabilities needed to do so could be unavailable due to being
  implemented by built-in modules which themselves are not yet
  initialized, or have already been torn down.

  This module is designed to help with that problem, by providing
  built-in support for doing on-the-fly reporting and/or background
  counting of inter-locale communication operations during module
  initialization and teardown.  To use it, set either or both of the
  config params below using appropriate ``-sconfigParamName=value``
  command line options when you compile your program.

  The reporting and/or counting enabled by this module covers all of
  program execution, from just before the first module is initialized
  until just after the last one is torn down.  This is almost always a
  superset of the part of the program that is of interest, which is
  often just a single module.  To learn what communication is being
  done by a single module during its initialization and teardown it is
  usually necessary to run a small test program twice, once with the
  module present and once without it.

  *Note:* It is not necessary to ``use`` this module.  It is included
  in every program automatically.  Using compiler command line options
  to set the config params it provides is the only way to interact
  with it.
 */
module startInitCommDiags {
  /*
    If this is set, on-the-fly reporting of communication operations
    will be turned on before any module initialization begins and
    turned off after all module teardown ends.  See module
    :mod:`CommDiagnostics` and its procedures :proc:`startVerboseComm`
    and :proc:`stopVerboseComm` for more information.
   */
  config param printInitVerboseComm = false;

  /*
    If this is set, communication operations are counted from before
    any module initialization begins until after all module teardown
    ends, and then the aggregate counts are printed.  See module
    :mod:`CommDiagnostics` and its procedures
    :proc:`startCommDiagnostics`, :proc:`stopCommDiagnostics`, and
    :proc:`getCommDiagnostics` for more information.
   */
  config param printInitCommCounts = false;

  if printInitVerboseComm {
    use CommDiagnostics;
    startVerboseComm();
  }

  if printInitCommCounts {
    use CommDiagnostics;
    startCommDiagnostics();
  }
}

