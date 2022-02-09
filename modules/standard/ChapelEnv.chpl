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

//
// ChapelEnv.chpl
//

/*

Access to Chapel Environment Variables

The values of Chapel's environment variables upon compile time are
accessible through the built-in parameters shown below. This information
can also be displayed from the command line by executing the compiled
program with the ``--about`` flag.

 */
pragma "module included by default"
module ChapelEnv {
  private use String;
  private use ChapelStandard;

  private inline proc warnChplConfigChange() {
    compilerWarning("In an upcoming release, you will need to 'use'/'import " +
                    "ChplConfig' in order to access 'CHPL_*' params",
                    errorDepth=2);
  }

  /* See :ref:`readme-chplenv.CHPL_HOME` for more information. */
  pragma "last resort"
  proc CHPL_HOME:string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_HOME;
  }

  /* Deprecated */
  pragma "last resort"
  deprecated "CHPL_AUX_FILESYS is deprecated, please let us know if this is a problem"
  proc CHPL_AUX_FILESYS param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_AUX_FILESYS;
  }

  /* See :ref:`readme-chplenv.CHPL_TARGET_PLATFORM` for more information. */
  pragma "last resort"
  proc CHPL_TARGET_PLATFORM param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_TARGET_PLATFORM;
  }

  /* See :ref:`readme-chplenv.CHPL_HOST_PLATFORM` for more information. */
  pragma "last resort"
  proc CHPL_HOST_PLATFORM param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_HOST_PLATFORM;
  }

  /* See :ref:`readme-chplenv.CHPL_HOST_ARCH` for more information. */
  pragma "last resort"
  proc CHPL_HOST_ARCH param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_HOST_ARCH;
  }

  /* See :ref:`readme-chplenv.CHPL_COMPILER` for more information. */
  pragma "last resort"
  proc CHPL_HOST_COMPILER param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_HOST_COMPILER;
  }

  /* See :ref:`readme-chplenv.CHPL_COMPILER` for more information. */
  pragma "last resort"
  proc CHPL_TARGET_COMPILER param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_TARGET_COMPILER;
  }

  /* See :ref:`readme-chplenv.CHPL_TARGET_ARCH` for more information. */
  pragma "last resort"
  proc CHPL_TARGET_ARCH param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_TARGET_ARCH;
  }

  /* See :ref:`readme-chplenv.CHPL_TARGET_CPU` for more information. */
  pragma "last resort"
  proc CHPL_TARGET_CPU param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_TARGET_CPU;
  }

  /* See :ref:`readme-chplenv.CHPL_LOCALE_MODEL` for more information. */
  pragma "last resort"
  proc CHPL_LOCALE_MODEL param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_LOCALE_MODEL;
  }

  /* See :ref:`readme-chplenv.CHPL_COMM` for more information. */
  pragma "last resort"
  proc CHPL_COMM param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_COMM;
  }

  /* See :ref:`readme-launcher` for more information. */
  pragma "last resort"
  proc CHPL_COMM_SUBSTRATE param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_COMM_SUBSTRATE;
  }

  /* See :ref:`readme-multilocale` for more information. */
  pragma "last resort"
  proc CHPL_GASNET_SEGMENT param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_GASNET_SEGMENT;
  }

  pragma "no doc"
  /* See :ref:`readme-chplenv.CHPL_LIBFABRIC` for more information. */
  pragma "last resort"
  proc CHPL_LIBFABRIC param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_LIBFABRIC;
  }

  /* See :ref:`readme-chplenv.CHPL_TASKS` for more information. */
  pragma "last resort"
  proc CHPL_TASKS param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_TASKS;
  }

  /* See :ref:`readme-chplenv.CHPL_LAUNCHER` for more information. */
  pragma "last resort"
  proc CHPL_LAUNCHER param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_LAUNCHER;
  }

  /* See :ref:`readme-chplenv.CHPL_TIMERS` for more information. */
  pragma "last resort"
  proc CHPL_TIMERS param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_TIMERS;
  }

  /* See :ref:`readme-chplenv.CHPL_UNWIND` for more information. */
  pragma "last resort"
  proc CHPL_UNWIND param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_UNWIND;
  }

  /* See :ref:`readme-chplenv.CHPL_MEM` for more information. */
  pragma "last resort"
  proc CHPL_MEM param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_MEM;
  }

  /* See :ref:`readme-chplenv.CHPL_MAKE` for more information. */
  pragma "last resort"
  proc CHPL_MAKE param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_MAKE;
  }

  /* See :ref:`readme-chplenv.CHPL_ATOMICS` for more information. */
  pragma "last resort"
  proc CHPL_ATOMICS param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_ATOMICS;
  }

  /* See :ref:`readme-atomics` for more information. */
  pragma "last resort"
  proc CHPL_NETWORK_ATOMICS param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_NETWORK_ATOMICS;
  }

  /* See :ref:`readme-chplenv.CHPL_GMP` for more information. */
  pragma "last resort"
  proc CHPL_GMP param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_GMP;
  }

  /* See :ref:`readme-chplenv.CHPL_HWLOC` for more information. */
  pragma "last resort"
  proc CHPL_HWLOC param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_HWLOC;
  }

  pragma "no doc"
  /* See :ref:`readme-chplenv.CHPL_JEMALLOC` for more information. */
  pragma "last resort"
  proc CHPL_JEMALLOC param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_JEMALLOC;
  }

  pragma "no doc"
  pragma "last resort"
  deprecated "CHPL_REGEXP is deprecated, please use CHPL_RE2 instead"
  proc CHPL_REGEXP param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_REGEXP;
  }

  /* See :ref:`readme-chplenv.CHPL_RE2` for more information. */
  pragma "last resort"
  proc CHPL_RE2 param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_RE2;
  }

  /* See :ref:`readme-chplenv.CHPL_LLVM` for more information. */
  pragma "last resort"
  proc CHPL_LLVM param : string {
    use ChplConfig;
    warnChplConfigChange();
    return CHPL_LLVM;
  }
}
