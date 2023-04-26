/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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
Access to configuration information for the 'chpl' compiler.

This module's contents provide access to compile-time aspects of
Chapel's configuration, such as those specified by ``CHPL_*``
environment variables or ``chpl`` command-line options, as well as
those inferred from the environment or other settings.  For details,
refer to :ref:`readme-chplenv` and the :ref:`chpl man page
<man-chpl>`.  This information can also be displayed from a Chapel
executable's command line by executing it with the ``--about`` flag.
*/

module ChplConfig {
  private use String;
  private use ChapelStandard;

  /* See :ref:`readme-chplenv.CHPL_HOME` for more information. */
  param CHPL_HOME:string;
  CHPL_HOME = __primitive("get compiler variable", "CHPL_HOME");

  /* Deprecated */
  @deprecated(notes="CHPL_AUX_FILESYS is deprecated, please let us know if this is a problem")
  proc CHPL_AUX_FILESYS param :string {
    // use a proc here because the split initialization caused an
    // additional deprecation warning
    return __primitive("get compiler variable", "CHPL_AUX_FILESYS");
  }

  /* See :ref:`readme-chplenv.CHPL_TARGET_PLATFORM` for more information. */
  param CHPL_TARGET_PLATFORM:string;
  CHPL_TARGET_PLATFORM = __primitive("get compiler variable", "CHPL_TARGET_PLATFORM");

  /* See :ref:`readme-chplenv.CHPL_HOST_PLATFORM` for more information. */
  param CHPL_HOST_PLATFORM:string;
  CHPL_HOST_PLATFORM = __primitive("get compiler variable", "CHPL_HOST_PLATFORM");

  /* See :ref:`readme-chplenv.CHPL_HOST_ARCH` for more information. */
  param CHPL_HOST_ARCH:string;
  CHPL_HOST_ARCH = __primitive("get compiler variable", "CHPL_HOST_ARCH");

  /* See :ref:`readme-chplenv.CHPL_COMPILER` for more information. */
  param CHPL_HOST_COMPILER:string;
  CHPL_HOST_COMPILER = __primitive("get compiler variable", "CHPL_HOST_COMPILER");

  /* See :ref:`readme-chplenv.CHPL_COMPILER` for more information. */
  param CHPL_TARGET_COMPILER:string;
  CHPL_TARGET_COMPILER = __primitive("get compiler variable", "CHPL_TARGET_COMPILER");

  /* See :ref:`readme-chplenv.CHPL_TARGET_ARCH` for more information. */
  param CHPL_TARGET_ARCH:string;
  CHPL_TARGET_ARCH = __primitive("get compiler variable", "CHPL_TARGET_ARCH");

  /* See :ref:`readme-chplenv.CHPL_TARGET_CPU` for more information. */
  param CHPL_TARGET_CPU:string;
  CHPL_TARGET_CPU = __primitive("get compiler variable", "CHPL_TARGET_CPU");

  /* See :ref:`readme-chplenv.CHPL_LOCALE_MODEL` for more information. */
  param CHPL_LOCALE_MODEL:string;
  CHPL_LOCALE_MODEL = __primitive("get compiler variable", "CHPL_LOCALE_MODEL");

  /* See :ref:`readme-chplenv.CHPL_COMM` for more information. */
  param CHPL_COMM:string;
  CHPL_COMM = __primitive("get compiler variable", "CHPL_COMM");

  /* See :ref:`readme-launcher` for more information. */
  param CHPL_COMM_SUBSTRATE:string;
  CHPL_COMM_SUBSTRATE = __primitive("get compiler variable", "CHPL_COMM_SUBSTRATE");

  /* See :ref:`readme-multilocale` for more information. */
  param CHPL_GASNET_SEGMENT:string;
  CHPL_GASNET_SEGMENT = __primitive("get compiler variable", "CHPL_GASNET_SEGMENT");

  pragma "no doc"
  /* See :ref:`readme-chplenv.CHPL_LIBFABRIC` for more information. */
  param CHPL_LIBFABRIC:string;
  CHPL_LIBFABRIC = __primitive("get compiler variable", "CHPL_LIBFABRIC");

  /* See :ref:`readme-chplenv.CHPL_TASKS` for more information. */
  param CHPL_TASKS:string;
  CHPL_TASKS = __primitive("get compiler variable", "CHPL_TASKS");

  /* See :ref:`readme-chplenv.CHPL_LAUNCHER` for more information. */
  param CHPL_LAUNCHER:string;
  CHPL_LAUNCHER = __primitive("get compiler variable", "CHPL_LAUNCHER");

  /* See :ref:`readme-chplenv.CHPL_TIMERS` for more information. */
  param CHPL_TIMERS:string;
  CHPL_TIMERS = __primitive("get compiler variable", "CHPL_TIMERS");

  /* See :ref:`readme-chplenv.CHPL_UNWIND` for more information. */
  param CHPL_UNWIND:string;
  CHPL_UNWIND = __primitive("get compiler variable", "CHPL_UNWIND");

  /* See :ref:`readme-chplenv.CHPL_MEM` for more information. */
  param CHPL_MEM:string;
  CHPL_MEM = __primitive("get compiler variable", "CHPL_MEM");

  /* See :ref:`readme-chplenv.CHPL_MAKE` for more information. */
  param CHPL_MAKE:string;
  CHPL_MAKE = __primitive("get compiler variable", "CHPL_MAKE");

  /* See :ref:`readme-chplenv.CHPL_ATOMICS` for more information. */
  param CHPL_ATOMICS:string;
  CHPL_ATOMICS = __primitive("get compiler variable", "CHPL_ATOMICS");

  /* See :ref:`readme-atomics` for more information. */
  param CHPL_NETWORK_ATOMICS:string;
  CHPL_NETWORK_ATOMICS = __primitive("get compiler variable", "CHPL_NETWORK_ATOMICS");

  /* See :ref:`readme-chplenv.CHPL_GMP` for more information. */
  param CHPL_GMP:string;
  CHPL_GMP = __primitive("get compiler variable", "CHPL_GMP");

  /* See :ref:`readme-chplenv.CHPL_HWLOC` for more information. */
  param CHPL_HWLOC:string;
  CHPL_HWLOC = __primitive("get compiler variable", "CHPL_HWLOC");

  pragma "no doc"
  /* See :ref:`readme-chplenv.CHPL_JEMALLOC` for more information. */
  param CHPL_JEMALLOC:string;
  CHPL_JEMALLOC = __primitive("get compiler variable", "CHPL_TARGET_JEMALLOC");

  /* See :ref:`readme-chplenv.CHPL_RE2` for more information. */
  param CHPL_RE2:string;
  CHPL_RE2 = __primitive("get compiler variable", "CHPL_RE2");

  /* See :ref:`readme-chplenv.CHPL_LLVM` for more information. */
  param CHPL_LLVM:string;
  CHPL_LLVM = __primitive("get compiler variable", "CHPL_LLVM");

  pragma "no doc"
  param CHPL_GPU_MEM_STRATEGY:string;
  CHPL_GPU_MEM_STRATEGY = __primitive("get compiler variable", "CHPL_GPU_MEM_STRATEGY");

  pragma "no doc"
  param CHPL_GPU_CODEGEN:string;
  CHPL_GPU_CODEGEN = __primitive("get compiler variable", "CHPL_GPU_CODEGEN");
}
